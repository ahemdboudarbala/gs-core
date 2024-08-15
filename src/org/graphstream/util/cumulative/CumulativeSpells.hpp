#ifndef CUMULATIVE_SPELLS_HPP
#define CUMULATIVE_SPELLS_HPP

#include <vector>
#include <string>
#include <sstream>
#include <limits>

class CumulativeSpells {
public:
    class Spell {
    public:
        Spell(double start, bool startOpen, double end, bool endOpen);
        Spell(double start, double end);
        Spell(double start);

        double getStartDate() const;
        double getEndDate() const;
        bool isStartOpen() const;
        bool isEndOpen() const;
        bool isStarted() const;
        bool isEnded() const;

        void setStartOpen(bool open);
        void setEndOpen(bool open);

        void* getAttachedData() const;
        void setAttachedData(void* data);

        std::string toString() const;

    private:
        double start;
        double end;

        bool startOpen;
        bool endOpen;

        bool closed;

        void* data;
    };

    CumulativeSpells();

    Spell* startSpell(double date);
    void updateCurrentSpell(double date);
    Spell* closeSpell();
    Spell* getCurrentSpell() const;
    Spell* getSpell(int i) const;
    int getSpellCount() const;

    Spell* getOrCreateSpell(double date);
    bool isEternal() const;

    std::string toString() const;

private:
    std::vector<Spell> spells;
    double currentDate;
};

#endif // CUMULATIVE_SPELLS_HPP
