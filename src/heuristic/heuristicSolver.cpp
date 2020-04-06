/*
 * heuristicSolver.cpp
 *
 *  Created on: 9 mars 2020
 *      Author: Valars
 */

#include "heuristicSolver.h"

#include "../model/constraint/ConstraintDaysSeq.h"
#include "../model/constraint/ConstraintInvolved.h"
#include "../model/constraint/ConstraintSeqMinMax.h"
using namespace std;

heuristicSolver::heuristicSolver() {
	// TODO Auto-generated constructor stub

}

heuristicSolver::~heuristicSolver() {
	// TODO Auto-generated destructor stub
}

//Algorithme glouton prenant en compte uniquement les contraintes de postes par jour et d'amplitude horaire sur les agents.
Model heuristicSolver::greedy(const Model m) {
	Model mr = Model(m);

	unsigned seed = chrono::system_clock::now().time_since_epoch().count();

	auto day = mr.getFirstDay();

	//Pour chaque jour
	int indiceWeek = day; //Indice de la semaine en cours
	for(int i=0;i<mr.getNbDays();i++){

		for(auto s : mr.getServices()){

			//On m�lange la liste des agents.
			vector<Agent*> agents = mr.getAgentFrom(s);
			shuffle(agents.begin(),agents.end(),default_random_engine(seed));
			shuffle(agents.begin(),agents.end(),default_random_engine(seed));

			//On r�cup�re les postes necessaires pour le jour day (la variable)
			auto required = s->getPostRequired()[day];
			for(auto a : agents){

				//Si la journ�e n'est affect�e � aucun poste
				if(a->getCalendar()[i]==NULL){
					//On regarde les postes restants � attribuer
					for(auto r : required){
						if(r.second>0){
							//On respecte les heures par semaine et par mois
							if(a->getWorkingHoursMonth()+r.first->getTime()<=a->getNbHoursMonth()+mr.getOvertime()
									&& a->getWorkingHoursWeek(mr.getFirstDay(),indiceWeek/7)+ r.first->getTime() <= a->getNbHoursWeek()){
								a->setCalendarDay(r.first,i);
								required[r.first]--; //on enl�ve un de ces postes de ceux disponibles
								break;
							}
						}
					}
				}
			}
		}
		day = mr.getNextDay(day);
		indiceWeek++;
	}
	heuristicSolver::nullTo(&mr,mr.getDefaultPost());
	return mr;
}

void heuristicSolver::nullTo(Model* m, Post* post) {
	for(auto s : m->getServices()){
		for(auto a : m->getAgentFrom(s)){
			for(int i=0;i<m->getNbDays();i++){
				if(a->getCalendar()[i]==NULL){
					a->setCalendarDay(post,i);
				}
			}
		}
	}
}

int heuristicSolver::check(Model* m, bool checkALL, bool log) {

	int score = 0;

	bool isValide = true;

	for(auto s : m->getServices()){
		for(auto a : m->getAgentFrom(s)){
			//Check des heures au mois pour les agents
			if(a->getWorkingHoursMonth() > a->getNbHoursMonth()+m->getOvertime()){
				if(log)
					cout << "Checker: D�passement d'heure au mois pour l'agent " << a->getId() << endl;
				isValide = false;
				if (checkALL)
					return false;

				score -= 1;
			}
			//Check des heures � la semaine pour les agents
			for(int i=0;i<6;i++){
				if(a->getWorkingHoursWeek(m->getFirstDay(),i) > a->getNbHoursWeek()){
					if (log)
						cout << "Checker: D�passement d'heure � la semaine " << i << " pour l'agent " << a->getId() << endl;
					isValide = false;
					if(checkALL)
						return false;

					score -= 1;
				}
			}

			for(auto c : s->getConstraints()){
				if(typeid(*c) == typeid(ConstraintDaysSeq)){
						score -= ((ConstraintDaysSeq*)c)->check(a, true, log);
				}
				else if (typeid(*c) == typeid(ConstraintInvolved)) {
						score -= ((ConstraintInvolved*)c)->check(a, true, log);
				}
				else if (typeid(*c) == typeid(ConstraintSeqMinMax)) {
						score -= ((ConstraintSeqMinMax*)c)->check(a, true, m->getFirstDay(), log);
				}
			}
		}
	}


	if(log)
		cout << "Score: " << score << endl;
	return score;
}
