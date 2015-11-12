#pragma once

#include "exp_lottery.h"

#pragma warning( push )
#pragma warning (disable:4251)

namespace silanis
{

  class EXT_LOTTERY CLotteryI
  {
  protected:// DATA
    double m_dPrizeFund;
    vector<string> m_lstWinners;
    // Error
    string m_errMsg;
    //
  public:
    CLotteryI(double dPrizeFund);
    virtual ~CLotteryI();
    //
    // get last error message
    virtual const string& getLastError() const { return m_errMsg; }

    // clear Participants, Winners, update PrizeFund
    virtual bool resetDraw() = 0;
    // add funds to prize, or substract funds (if dFunds is negative), could return false in case if there is no sufficient funds (check getLastError())
    virtual bool addFunds(double dFunds);

    // getting current Prize Fund
    virtual double getPrizeFund() const { return m_dPrizeFund; }

    // purchase lottery ticket ($10), add participants to List
    virtual bool purchase(string sName, size_t &iBallNumb) = 0;
    // execute draw in Lottery, define winners
    virtual bool draw() = 0;
    // get list of winners
    virtual const vector<string>& getWinners() const { return m_lstWinners; }
    // get prize for place
    virtual double getPrizeAmountByPlace(size_t iPlace) const;
  };

}

#pragma warning( pop )

