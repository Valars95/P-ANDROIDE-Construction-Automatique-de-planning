/*
 * ConstraintInvolved.h
 *
 *  Created on: 21 f�vr. 2020
 *      Author: Valars
 */

#ifndef SRC_MODEL_CONSTRAINT_CONSTRAINTINVOLVED_H_
#define SRC_MODEL_CONSTRAINT_CONSTRAINTINVOLVED_H_

#include "Constraint.h"
#include "../Agent.h"
//Correspondant à la contrainte C6


class ConstraintInvolved: public Constraint {
public:
	ConstraintInvolved(std::vector<Post*> first, std::vector<Post*> last, int priority);
	ConstraintInvolved(std::vector<string> first, std::vector<string> last, int firstDay, int priority);
	virtual ~ConstraintInvolved();

	const std::vector<Post*>& getFirstSeq() const;
	void setFirstSeq(const std::vector<Post*> &firstSeq);
	const std::vector<Post*>& getLastSeq() const;
	void setLastSeq(const std::vector<Post*> &lastSeq);

	const std::vector<string>& getFirstSeqAtt() const;
	void setFirstSeqAtt(const std::vector<string>& firstSeq);
	const std::vector<string>& getLastSeqAtt() const;
	void setLastSeqAtt(const std::vector<string>& lastSeq);

	string getSeqToPrint();

	int getPriority();
	int getFirstDay();

	int check(const Agent* agent, int day, bool checkALL, bool log);
	std::vector < std::pair<std::pair<int, int>, std::pair<int, int>>> checkValuation(const Agent* agent, int day);

protected:
	int priority;
	int firstDay;
	std::vector<Post*> firstSeq; //Première partie de l'implication
	std::vector<Post*> lastSeq; //Seconde partie de l'implication
	std::vector<string> firstSeqAtt;
	std::vector<string> lastSeqAtt;
};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINTINVOLVED_H_ */
