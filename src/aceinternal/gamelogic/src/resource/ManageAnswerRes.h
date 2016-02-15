#ifndef __MANAGE_ANSWER_RES_H_
#define __MANAGE_ANSWER_RES_H_

#include "AnswerStruct.h"
#include "AnswerStructMap.h"

#pragma warning(push, 2)
#include "typedef.h"
#pragma warning(pop)

#include <boost/ptr_container/ptr_map.hpp>

class Element;
class Document;

class ManageAnswerRes {
public:
    bool Load(Document *quest, Document *event, Document *config, Document *coins);

    AnswerResource const& getAnswerResource();

private:
    AnswerResource res;

    bool loadEvents(Document *doc);
    bool loadQuestions(Document *doc);
    bool loadCoins(Document *);
    bool loadConfig(Document *);

    bool readEvent(Element *ele);
    bool readQuestion(Element *ele);

    typedef boost::ptr_map<uint32, AnswerEvent> EventContainer;
    typedef boost::ptr_map<uint32, AnswerSection> SectionContainer;
    typedef boost::ptr_map<uint32, AnswerQuestion> QuestionContainer;

    EventContainer event_container;
    SectionContainer section_container;
    QuestionContainer question_container;
};

#endif
