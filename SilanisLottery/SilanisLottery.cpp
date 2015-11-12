// SilanisLottery.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <time.h>

#include "LotteryBasic.h"
#include "LotteryAdv.h"
#include "Tools.h"

using namespace silanis;

void init()
{
  /* initialize random seed: */
  srand((unsigned int)time(NULL));
}
void printHeader()
{
  cout 
    << "Silanis Lottery" << endl << endl
    << "Available commands: Buy, Draw, Winners, Reset, Funds, Help, Exit" << endl
    << "  Buy     - buy ticket (format: buy <name>)" << endl
    << "  Draw    - execute draw" << endl
    << "  Winners - display the Winners of last draw" << endl
    << "  Reset   - prepare lottery for new iteration" << endl
    << "  Funds   - get current Prize funds" << endl
    << "  Help    - show HELP screen" << endl
    << "  Exit    - exit from application" << endl << endl
    ;
}

int doLottery(CLotteryI &lottery)
{
  string sCmd;
  lottery.resetDraw();
  bool bError = false;
  bool bExit = false;
  for (;!bExit;) 
  {
    if (bError) {
      cout << lottery.getLastError() << endl;
      bError = false;
    }
    cout << "Enter command: ";
    getline(cin, sCmd);
    trim_inplace(sCmd);
    if (sCmd.empty()) continue;
    // convert command to lower case
    transform(sCmd.begin(), sCmd.end(), sCmd.begin(), ::tolower);

    // buy
    if (sCmd.compare("buy") == 0)
    {
      string sName;
      cout << "Enter name of purchaser: ";
      getline(cin, sName);
      trim_inplace(sCmd);
      if (sCmd.empty()) { 
        cout << "ERROR: Purchaser name should not be empty" << endl;
        continue;
      }
      size_t iBallNumb = 0;
      if (!lottery.purchase(sName, iBallNumb)) { bError = true; continue; }
      cout << sName << " purchased ball #" << iBallNumb << endl;
      continue;
    }
    // draw
    if (sCmd.compare("draw") == 0)
    {
      if (!lottery.draw()) { bError = true; /*bExit = true;*/ continue; }
      cout << "Draw executed and winners selected. Type command Winners to see them." << endl;
      continue;
    }
    // reset draw
    if (sCmd.compare("reset") == 0)
    {
      if (!lottery.resetDraw()) { bError = true; /*bExit = true;*/ continue; }
      cout << "Lottery read for new iteration." << endl;
      continue;
    }
    // display Winners
    if (sCmd.compare("winners") == 0)
    {
      cout << "Winners:" << endl;
      const vector<string> &lstWinners= lottery.getWinners();
      if (!lstWinners.empty())
      {
        size_t i = 0;
        for (auto sName : lstWinners)
        {
          i++;
          cout << i << " ball - " << sName << ": " << int(lottery.getPrizeAmountByPlace(i)) << "$" << endl;
        }
      }
      else
      {
        cout << "<empty list>" << endl;
      }
      continue;
    }
    // Funds
    if (sCmd.compare("funds") == 0)
    {
      cout << "Lottery Prize Funds: "<< int(lottery.getPrizeFund()) << endl;
      continue;
    }
    // Help
    if (sCmd.compare("help") == 0)
    {
      printHeader();
      continue;
    }
    // Exit
    if (sCmd.compare("exit") == 0)
    {
      cout << "Exit application." << endl;
      bExit = true; 
      continue;
    }
    
    {// bad command
      cout << "Invalid command: " << sCmd << endl;
    }
  }
  return 0;
}

int main()
{
  init();
  printHeader();
  //
  //CLotteryBasic lottery;
  CLotteryAdv lottery;
  return doLottery(lottery);
}

