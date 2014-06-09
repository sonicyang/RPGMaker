#ifndef SKILL_H
#define SKILL_H

#include <string>

class Skill
{
    public:
        Skill();
        Skill(std::string name);
        virtual ~Skill();

        std::string getName() { return _name; }
        void setName(std::string val) { _name = val; }
        std::string getDescription() { return _description; }
        void setDescription(std::string val) { _description = val; }

        int getMPC() { return _MPC; }
        void setMPC(int val) { _MPC = val; }

        int geteTarget() { return _eTarget; }
        void seteTarget(int val) { _eTarget = val; }
        int geteHPv() { return _eHPv; }
        void seteHPv(int val) { _eHPv = val; }
        int geteMPv() { return _eMPv; }
        void seteMPv(int val) { _eMPv = val; }
        int geteATKv() { return _eATKv; }
        void seteATKv(int val) { _eATKv = val; }
        int geteDEFv() { return _eDEFv; }
        void seteDEFv(int val) { _eDEFv = val; }

        int getfTarget() { return _fTarget; }
        void setfTarget(int val) { _fTarget = val; }
        int getfHPv() { return _fHPv; }
        void setfHPv(int val) { _fHPv = val; }
        int getfMPv() { return _fMPv; }
        void setfMPv(int val) { _fMPv = val; }
        int getfATKv() { return _fATKv; }
        void setfATKv(int val) { _fATKv = val; }
        int getfDEFv() { return _fDEFv; }
        void setfDEFv(int val) { _fDEFv = val; }

    protected:
    private:
        std::string _name = "Null";
        std::string _description = "Null";
        int _MPC = 0;

        int _eTarget = -1;
        int _eHPv = 0;
        int _eMPv = 0;
        int _eATKv = 0;
        int _eDEFv = 0;

        int _fTarget = -1;
        int _fHPv = 0;
        int _fMPv = 0;
        int _fATKv = 0;
        int _fDEFv = 0;
};

#endif // SKILL_H
