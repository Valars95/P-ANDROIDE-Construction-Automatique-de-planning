/*
 * ConstraintInvolved.h
 *
 *  Created on: 21 f�vr. 2020
 *      Author: Valars
 */

#ifndef SRC_MODEL_CONSTRAINT_CONSTRAINTINVOLVED_H_
#define SRC_MODEL_CONSTRAINT_CONSTRAINTINVOLVED_H_

#include "Constraint.h"
#include "Post.h"
#include <vector>

//Correspondant � la contrainte C6

class ConstraintInvolved: public Constraint {
public:
	ConstraintInvolved(std::vector<Post> first, std::vector<Post> last, int priority);
	virtual ~ConstraintInvolved();
	const std::vector<Post>& getFirstSeq() const;
	void setFirstSeq(const std::vector<Post> &firstSeq);
	const std::vector<Post>& getLastSeq() const;
	void setLastSeq(const std::vector<Post> &lastSeq);

protected:
	int priority;
	std::vector<Post> firstSeq; //Premi�re partie de l'implication
	std::vector<Post> lastSeq; //Second partie de l'implication
};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINTINVOLVED_H_ */
