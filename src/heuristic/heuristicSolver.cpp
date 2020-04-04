/*
 * heuristicSolver.cpp
 *
 *  Created on: 9 mars 2020
 *      Author: Valars
 */

#include "heuristicSolver.h"


using namespace std;

heuristicSolver::heuristicSolver() {
	// TODO Auto-generated constructor stub

}

heuristicSolver::~heuristicSolver() {
	// TODO Auto-generated destructor stub
}

//Algorithme glouton prenant en compte uniquement les contraintes de postes par jours et d'amplitude horaire sur les agents.
Model heuristicSolver::greedy(const Model m) {
	Model mr = Model(m);

	unsigned seed = chrono::system_clock::now().time_since_epoch().count();

	auto day = mr.getFirstDay();

	//Pour chaque jours
	int indiceWeek = day; //Indice de la semaine en cours
	for(int i=0;i<mr.getNbDays();i++){

		for(auto s : mr.getServices()){

			//On m�lange la liste des agents.
			vector<Agent*> v = mr.getAgentFrom(s);
			shuffle(v.begin(),v.end(),default_random_engine(seed));
			shuffle(v.begin(),v.end(),default_random_engine(seed));

			//On r�cup�re les postes necessaires pour le jour day
			auto required = s->getPostRequired()[day];
			for(auto a : v){

				//Si la journ�e est affect� � aucun poste
				if(a->getCalendar()[i]==NULL){
					//On regarde les poste restant � attribuer
					for(auto r : required){
						if(r.second>0){
							//On respecte les heures par semaine et par mois
							if(a->getWorkingHoursMonth()+r.first->getTime()<=a->getNbHoursMonth()+mr.getOvertime()
									&& a->getWorkingHoursWeek(mr.getFirstDay(),indiceWeek/7)+ r.first->getTime() <= a->getNbHoursWeek()){
								a->setCalendarDay(r.first,i);
								required[r.first] = r.second-1;
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

Model heuristicSolver::getNeighborSwap(Model* m)
{
	Model mr = Model(*m);

	srand(time(0));

	//Choix du jour
	int day = rand()%31;

	//On choisit un service al�atoirement
	int serviceI = rand()%mr.getServices().size();
	Service* service = NULL;
	int i = 0;
	for (auto s : mr.getServices()) {
		if (serviceI == i) {
			service = s;
			break;
		}
		i++;
	}

	//Choix des deux agents � swap
	int agent1 = rand() % mr.getAgentFrom(service).size();
	while(mr.getAgentFrom(service)[agent1]->getCalendarLock()[day] == true)
		agent1 = rand() % mr.getAgentFrom(service).size();

	int agent2 = rand() % mr.getAgentFrom(service).size();
	while (mr.getAgentFrom(service)[agent2]->getCalendarLock()[day] == true || agent1 == agent2 ||
		mr.getAgentFrom(service)[agent1]->getCalendar()[day] == mr.getAgentFrom(service)[agent2]->getCalendar()[day])
		agent2 = rand() % mr.getAgentFrom(service).size();

	cout << "Swap agent " << mr.getAgentFrom(service)[agent1]->getId() << " et agent " << mr.getAgentFrom(service)[agent2]->getId() << " jour " << day+1 << endl;

	//On swap les poste des deux agents choisit
	Post* tmp = mr.getAgentFrom(service)[agent1]->getCalendar()[day];
	mr.getAgentFrom(service)[agent1]->setCalendarDay(mr.getAgentFrom(service)[agent2]->getCalendar()[day], day);
	mr.getAgentFrom(service)[agent2]->setCalendarDay(tmp, day);

	return mr;
}


