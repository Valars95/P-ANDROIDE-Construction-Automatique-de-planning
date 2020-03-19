/*
 * Agent.h
 *
 *  Created on: 8 f�vr. 2020
 *      Author: Valars
 */

#include <string>
#include "Service.h"
#include "constraint/Constraint.h"
#include "Model.h"

#ifndef MODEL_AGENT_H_
#define MODEL_AGENT_H_

enum Status {Beginner, Rooki, Confirmed};

class Agent {
public:
	Agent();
	Agent(string id, int nbHourMonth,int nbHoursWeek, Status status);
	Agent(const Agent &obj);
	virtual ~Agent();

	const string& getId() const;
	void setId(const string &id);

	float getNbHoursMonth() const;
	void setNbHoursMonth(float nbHours);

	const array<Post*, 31>& getCalendar() const;
	void setCalendar(const array<Post*, 31> &calendar);
	void setCalendarDay(Post* post, int day);

	const std::vector<Post*>& getImpossiblePosts() const;
	void setImpossiblePosts(const std::vector<Post*> &impossiblePosts);

	Service*& getService();
	void setService( Service *&service);

	const std::vector<Constraint*>& getConstraints() const;
	void setConstraints(const std::vector<Constraint*> &constraints);
	void addConstraint(Constraint* constraint);

	Status getStatus() const;
	void setStatus(Status status);
	const array<Post*, 7>& getLastMonthCalendar() const;
	void setLastMonthCalendar(const array<Post*, 7> &lastMonthCalendar);
	float getNbHoursWeek() const;
	void setNbHoursWeek(float nbHoursWeek);

	float getWorkingHoursMonth();
	float getWorkingHoursWeek(Day day, int weekI);

protected:
	string id;
	Status status;
	float nbHoursMonth;
	float nbHoursWeek;

	array<Post*, 31> calendar; //Liste o� chaque �l�ment = un jour (donc ordonn�) avec le poste attribu�
	array<Post*, 7> lastMonthCalendar; //Liste des 7 derniers postes avant le d�but du mois
	std::vector<Post*> impossiblePosts; // Liste des potes qui ne sont pas attribuables � l'agent (C4)

	std::vector<Constraint*> constraints;

	Service* service; //service pr�f�r�/affect�

};

#endif /* MODEL_AGENT_H_ */
