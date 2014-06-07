#ifndef BACKPACKITEM_H
#define BACKPACKITEM_H

#include <string>


class Item
{
    public:
        Item();
        Item(std::string);
        virtual ~Item();
        //Item(const Item& other);
        //Item& operator=(const Item& other);

        unsigned int getID() const { return _id; }
        std::string getName() const { return _name; }
        void setName(std::string val) { _name = val; }
        std::string getDescription() const { return _description; }
        void setDescription(std::string val) { _description = val; }
        int getSellPrice() const { return _price * 0.7; }
        int getBuyPrice() const { return _price; }
        void setPrice(int val) { _price = val; }

        bool isComsumable() const { return _iscomsumable; }
        void setComsumable(bool val) { _iscomsumable = val; }
        bool isUsable() const;

        int getHPVarient() const { return _HPv; }
        void setHPVarient(int val) { _HPv = val; }
        int getMPVarient() const { return _MPv; }
        void setMPVarient(int val) { _MPv = val; }
        int getATKVarient() const { return _ATKv; }
        void setATKVarient(int val) { _ATKv = val; }
        int getDEFVarient() const { return _DEFv; }
        void setDEFVarient(int val) { _DEFv = val; }

        int getType() const { return _type; }
        void setType(int val) { _type = val; }

        bool operator<(const Item&) const;
        bool operator<=(const Item&) const;
        bool operator>(const Item&) const;
        bool operator>=(const Item&) const;
        bool operator==(const Item&) const;
        bool operator!=(const Item&) const;

    protected:
    private:

        unsigned int _id = 0xffffffff;
        std::string _name = "Empty";
        std::string _description = "Empty";
        int _price = 0;
        bool _iscomsumable = 0;

        int _type = -1;

        int _HPv = 0;
        int _MPv = 0;
        int _ATKv = 0;
        int _DEFv = 0;
};

#endif // BACKPACKITEM_H
