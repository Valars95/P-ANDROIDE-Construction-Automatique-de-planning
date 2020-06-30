/*
 * ConstraintDaysSeq.cpp
 *
 *  Created on: 10 f�vr. 2020
 *      Author: Valars
 */

#include "ConstraintDaysSeq.h"

using namespace std;

ConstraintDaysSeq::ConstraintDaysSeq(vector<Post*> seq, int priority) {
	sequence = seq;
	this->priority = priority;

}

ConstraintDaysSeq::ConstraintDaysSeq(vector<string> seq, int priority) {
	sequenceAtt = seq;
	this->priority = priority;

}

ConstraintDaysSeq::~ConstraintDaysSeq() {
}

//! \return priority priority of the constraint

int ConstraintDaysSeq::getPriority()
{
	return priority;
}

void ConstraintDaysSeq::setPriority(int priority) {
	this->priority = priority;
}

const std::vector<Post*>& ConstraintDaysSeq::getSequence() const {
	return sequence;
}

void ConstraintDaysSeq::setSequence(const std::vector<Post*> &sequence) {
	this->sequence = sequence;
}

const std::vector<string>& ConstraintDaysSeq::getSequenceAtt() const {
	return sequenceAtt;
}

void ConstraintDaysSeq::setSequenceAtt(const std::vector<string> &sequenceAtt) {
	this->sequenceAtt = sequenceAtt;
}

string ConstraintDaysSeq::getSeqToPrint() {
	string s = "ConstraintDaySeq ";
	for(auto a : sequenceAtt){
		s+= a+" ";
	}
	return s;
}

//Retourne true si la séquence de la contrainte se retrouve dans le calendrier de l'agent
int ConstraintDaysSeq::check(const Agent *agent, bool checkALL, bool log) {

	unsigned int indice = 0;
	bool found = false;
	int i = 0;
	bool exist = false;
	int nb_fail = 0;
	bool first = false;
	//On prend en compte les 7 jours avant le debut du mois
	for(auto post : agent->getLastMonthCalendar()){
		if(post!=NULL){
			for(auto att : post->getAttributs()){
				if(att==sequenceAtt[indice]){
					found = true;
					indice++;
					//Si on arrive au bout de la séquence, alors elle est présente dans le calendrier
					if(indice>=sequenceAtt.size()){
						if(log)
							cout << getSeqToPrint() << ": Agent " << agent->getId() << " Jour "
									<< i-indice+2 << " � " << i+1 << endl;
						exist = true;
						found = false;
						nb_fail++;
						indice = 0;
					}
					break;
				}
				if (att == sequenceAtt[0]) {
					first = true;
				}
			}
			if (!found) {
				indice = 0;
				if (first)
					indice = 1;
			}
			found = false;
			first = false;
		}
		else
			indice=0;
		i++;
	}
	i=0;
	//On continue sur le calendrier
		for(auto post : agent->getCalendar()){
			if(post!=NULL){
				for(auto att : post->getAttributs()){
					if(!found && att==sequenceAtt[indice]){
						found = true;
						indice++;
						//Si on arrive au bout de la séquence, alors elle est présente dans le calendrier
						if(indice>=sequenceAtt.size()){
							if(log)
								cout << getSeqToPrint() << ": Agent " << agent->getId() << " Jour "
										<< i-indice+2 << " � " << i+1 << endl;
							exist = true;
							found = false;
							nb_fail++;
							indice = 0;
							if(!checkALL)
								return true;
						}
					}
					if (att == sequenceAtt[0]){
						first = true;
					}
				
				}
				if (!found) {
					indice = 0;
					if (first)
						indice = 1;
				}
				found = false;
				first = false;
			}
			else
				indice=0;
			i++;
		}
	return nb_fail*priority;
}

std::vector<std::pair<int, int>> ConstraintDaysSeq::checkValuation(const Agent* agent) {

	unsigned int indice = 0;
	bool found = false;
	int i = 0;
	bool exist = false;
	int nb_fail = 0;
	bool first = false;
	vector<pair<int, int>> v;

	//On prend en compte les 7 jours avant le debut du mois
	for (auto post : agent->getLastMonthCalendar()) {
		if (post != NULL) {
			for (auto att : post->getAttributs()) {
				if (att == sequenceAtt[indice]) {
					found = true;
					indice++;
					//Si on arrive au bout de la séquence, alors elle est présente dans le calendrier
					if (indice >= sequenceAtt.size()) {
						exist = true;
						found = false;
						v.push_back(pair<int,int>(i - indice+1, i));
						nb_fail++;
						indice = 0;
						
					}
					break;
				}
				if (att == sequenceAtt[0]) {
				first = true;
				}
			}
			if (!found) {
				indice = 0;
				if (first)
					indice = 1;
			}
			found = false;
			first = false;
		}
		else
		indice = 0;
		i++;
	}
	i = 0;
	//On continue sur le calendrier
	for (auto post : agent->getCalendar()) {
		if (post != NULL) {
			for (auto att : post->getAttributs()) {
				if (!found && att == sequenceAtt[indice]) {
					found = true;
					indice++;
					//Si on arrive au bout de la séquence, alors elle est présente dans le calendrier
					if (indice >= sequenceAtt.size()) {
						exist = true;
						found = false;
						v.push_back(pair<int, int>(i - indice + 1, i));
						nb_fail++;
						indice = 0;
					}
				}
				if (att == sequenceAtt[0]) {
					first = true;
				}
			}
			if (!found) {
				indice = 0;
				if (first)
					indice = 1;
			}
			found = false;
			first = false;
		}
		else
			indice = 0;
		i++;
	}
	return v;
}

