#include "stdafx.h"
#include "LotteryBasic.h"

using namespace silanis;

// clear Participants, Winners, update PrizeFund
bool CLotteryBasic::resetDraw()
{
  {// update PrizeFund
    double dWinnerMoney = 0.0;
    for (size_t iWinner = 0; iWinner < m_lstWinners.size(); iWinner++)
      dWinnerMoney += getPrizeAmountByPlace(iWinner + 1);
    addFunds(-dWinnerMoney);
  }
  //
  m_lstParticipants.clear();
  m_lstWinners.clear();
  return true;
}

// purchase lottery ticket ($10), add participants to List
bool CLotteryBasic::purchase(string sName, size_t &iBallNumb)
{
  return purchaseEx(sName, iBallNumb, 10.0);
}
bool CLotteryBasic::purchaseEx(string sName, size_t &iBallNumb, double dTicketCost)
{
  {// validate input
    {// validate name
      if (sName.empty())
      {// error
        m_errMsg = "Name should not be empty.";
        return false;
      }
      {// validate repeated names
        bool bFound = (std::find(m_lstParticipants.begin(), m_lstParticipants.end(), sName) != m_lstParticipants.end());
        if (bFound)
        {// error
          char msg[200];
          sprintf_s(msg, 200, "Name '%s' already bought ticket.", sName.c_str());
          m_errMsg = msg;
          return false;
        }
      }
    }
    if (dTicketCost < -0.0001)
    {// error
      m_errMsg = "Ticket cost should not be negative.";
      return false;
    }
  }
  iBallNumb = indexToBallNumb(m_lstParticipants.size());
  m_lstParticipants.push_back(sName);
  if (!addFunds(dTicketCost))
  {// error
    m_errMsg = "Internal error: in method addFunds()";
    return false;
  }
  return true;
}

// execute draw in Lottery, define winners, update PrizeFund
bool CLotteryBasic::draw()
{
  return drawEx(3);
}
bool CLotteryBasic::drawEx(size_t iWinnersCnt)
{
  // validate input
  if (m_lstWinners.size() > 0)
  {// error
    m_errMsg = "Invalid state of CLottery instance.";
    return false;
  }
  if (iWinnersCnt > m_lstParticipants.size())
  {// error
    char msg[200];
    sprintf_s(msg, 200, "Not enough participants (%i) to select %i winner(s)", m_lstParticipants.size(), iWinnersCnt);
    m_errMsg= msg;
    return false;
  }

  for (size_t iWinner = 0; iWinner < iWinnersCnt; iWinner++)
  {
    // winner selected by randomize based on the amount of participants (excluding the previously selected winners)
    int iWinnerInd = rand() % m_lstParticipants.size();
    string sWinnerName;
    if (!exctractItem(iWinnerInd, sWinnerName))
    {// error
      m_errMsg = "internal error";
      return false;
    }
    m_lstWinners.push_back(sWinnerName);
  }
  _ASSERT(m_lstWinners.size() == iWinnersCnt);
  return true;
}

bool CLotteryBasic::exctractItem(size_t ind, string &res)
{// scan 'm_lstParticipants' for Index and remove item from the list
  size_t i = 0;
  for (auto it = m_lstParticipants.cbegin(); it != m_lstParticipants.cend(); it++, i++)
  {
    if (i == ind)
    {
      res = *it;
      m_lstParticipants.erase(it);
      return true;
    }
  }
  return false;
}
