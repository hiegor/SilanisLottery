#pragma once

#include "LotteryI.h"

#pragma warning( push )
#pragma warning (disable:4251)

namespace silanis
{

  class EXT_LOTTERY CLotteryAdv : public CLotteryI
  {
  protected:// DATA
    map<string,size_t> m_mapParticipants;

  public:
    CLotteryAdv(double dPrizeFund = 200.0) :
      CLotteryI(dPrizeFund)
    {}
    virtual ~CLotteryAdv() {}

    // clear Participants, Winners, update PrizeFund
    virtual bool resetDraw() override;

    // purchase lottery ticket ($10), add participants to List
    virtual bool purchase(string sName, size_t &iBallNumb) override;
    // execute draw in Lottery, define winners
    virtual bool draw() override;

  protected:// helper
    bool purchaseEx(string sName, size_t &iBallNumb, double dTicketCost);
    bool drawEx(size_t iWinnersCnt);
    bool exctractItem(size_t ind, string &res);
    // convert index in the list into Ball Number
    static size_t indexToBallNumb(size_t iInd) { return iInd + 1; }
  };

}

#pragma warning( pop )
