#include "stdafx.h"
#include "LotteryI.h"

using namespace silanis;

CLotteryI::CLotteryI(double dPrizeFund):
  m_dPrizeFund(dPrizeFund)
{}
CLotteryI::~CLotteryI()
{}

bool CLotteryI::addFunds(double dFunds)
{
  double dPrizeFundNew = m_dPrizeFund + dFunds;
  if (dPrizeFundNew < 0) {
    // error
    m_errMsg = "No sufficient fund";
    return false;
  }
  m_dPrizeFund = dPrizeFundNew;
  return true;
}

double CLotteryI::getPrizeAmountByPlace(size_t iPlace) const
{
  double dRes = 0.0;
  double dPrizeFund = getPrizeFund() / 2;
  switch (iPlace)
  {
  case 1: dRes = dPrizeFund*0.75; break;
  case 2: dRes = dPrizeFund*0.15; break;
  case 3: dRes = dPrizeFund*0.10; break;
  default: dRes = 0.0;
  }
  return dRes;
}
