#include "prompt.h"
#include "genericcontorller.h"
#include "render.h"
#include "engine.h"
#include "utils.h"
#include "enum.h"

prompt::prompt(Engine* eng) :
    genericContorller(eng)
{
}

prompt::~prompt()
{
    //dtor
}

int prompt::hKeyZ(){
    discardMessage();
    engine->engineCall(loadStack(svc::restoreStat));
    return 0;
}

int prompt::hRender(render& rdr){
    if(hasMessage())
        rdr.render_prompt(*this);
    return 0;
}

void prompt::discardMessage(){
    if(m_message.size() > 0){
        m_message.pop_front();
        m_whom.pop_front();
    }
}
void prompt::loadMessaage(const wchar_t* m, const wchar_t* w){
    std::wstring s1(m);
    std::wstring s2(w);
    loadMessaage(s1, s2);
}
void prompt::loadMessaage(std::wstring m, std::wstring w){
    m_message.push_back(m);
    m_whom.push_back(w);
}
