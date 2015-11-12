#include "stdafx.h"
#include "CppUnitTest.h"

#include "LotteryBasic.h"
#include "LotteryAdv.h"
#include "tools.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace silanis;

#define ACCURACY 0.0000001

namespace Lottery_UT
{		
	TEST_CLASS(CLotteryUT)
	{
	public:
		
    CLotteryI* LotteryObj_Create(double dPrizeFund = 200.0)
    {
      //return new CLotteryBasic(dPrizeFund);
      return new CLotteryAdv(dPrizeFund);
    }

    TEST_METHOD(LotteryDraw_Basic)
    {
      size_t iBallNumb = 0;
      {// draw: 200
        std::unique_ptr<CLotteryI> pLottery(LotteryObj_Create(170.0));
        CLotteryI &lottery= *pLottery;
        Assert::IsTrue(lottery.purchase("Egor", iBallNumb));
        Assert::IsTrue(iBallNumb==1);
        Assert::IsTrue(lottery.purchase("Andre", iBallNumb));
        Assert::IsTrue(iBallNumb == 2);
        Assert::IsTrue(lottery.purchase("Dmitry", iBallNumb));
        Assert::IsTrue(iBallNumb == 3);
        Assert::IsTrue(lottery.draw());
        auto winners = lottery.getWinners();
        Assert::IsTrue(winners.size() == 3);
        Assert::AreEqual(75.0, lottery.getPrizeAmountByPlace(1));
        Assert::AreEqual(15.0, lottery.getPrizeAmountByPlace(2));
        Assert::AreEqual(10.0, lottery.getPrizeAmountByPlace(3));
      }
      {// draw: 300
        std::unique_ptr<CLotteryI> pLottery(LotteryObj_Create(170.0));
        CLotteryI &lottery = *pLottery;
        Assert::IsTrue(lottery.addFunds(200));
        Assert::IsTrue(lottery.purchase("Egor", iBallNumb));
        Assert::IsTrue(iBallNumb == 1);
        Assert::IsTrue(lottery.purchase("Andre", iBallNumb));
        Assert::IsTrue(iBallNumb == 2);
        Assert::IsTrue(lottery.purchase("Dmitry", iBallNumb));
        Assert::IsTrue(iBallNumb == 3);
        Assert::IsTrue(lottery.draw());
        auto winners = lottery.getWinners();
        Assert::IsTrue(winners.size() == 3);
        Assert::AreEqual(150.0, lottery.getPrizeAmountByPlace(1));
        Assert::AreEqual(30.0, lottery.getPrizeAmountByPlace(2));
        Assert::AreEqual(20.0, lottery.getPrizeAmountByPlace(3));
      }
      {// draw: 2 times
        std::unique_ptr<CLotteryI> pLottery(LotteryObj_Create());
        CLotteryI &lottery = *pLottery;
        // first time
        Assert::IsTrue(lottery.purchase("Egor", iBallNumb));
        Assert::IsTrue(iBallNumb == 1);
        Assert::IsTrue(lottery.purchase("Andre", iBallNumb));
        Assert::IsTrue(iBallNumb == 2);
        Assert::IsTrue(lottery.purchase("Dmitry", iBallNumb));
        Assert::IsTrue(iBallNumb == 3);
        Assert::IsTrue(lottery.draw());
        // second time
        Assert::IsTrue(lottery.resetDraw());
        Assert::IsTrue(lottery.purchase("Egor", iBallNumb));
        Assert::IsTrue(iBallNumb == 1);
        Assert::IsTrue(lottery.purchase("Andre", iBallNumb));
        Assert::IsTrue(iBallNumb == 2);
        Assert::IsTrue(lottery.purchase("Dmitry", iBallNumb));
        Assert::IsTrue(iBallNumb == 3);
        Assert::IsTrue(lottery.draw());
        //
        Assert::AreEqual(54.375, lottery.getPrizeAmountByPlace(1));
        Assert::AreEqual(10.875, lottery.getPrizeAmountByPlace(2));
        Assert::AreEqual(7.25, lottery.getPrizeAmountByPlace(3));
      }

      {// draw: 5 participants
        std::unique_ptr<CLotteryI> pLottery(LotteryObj_Create());
        CLotteryI &lottery = *pLottery;
        // first time
        Assert::IsTrue(lottery.purchase("Egor", iBallNumb));
        Assert::IsTrue(iBallNumb == 1);
        Assert::IsTrue(lottery.purchase("Andre", iBallNumb));
        Assert::IsTrue(iBallNumb == 2);
        Assert::IsTrue(lottery.purchase("Dmitry", iBallNumb));
        Assert::IsTrue(iBallNumb == 3);
        Assert::IsTrue(lottery.purchase("John", iBallNumb));
        Assert::IsTrue(iBallNumb == 4);
        Assert::IsTrue(lottery.purchase("Lisa", iBallNumb));
        Assert::IsTrue(iBallNumb == 5);
        Assert::IsTrue(lottery.draw());
        //
        Assert::AreEqual(93.75, lottery.getPrizeAmountByPlace(1));
        Assert::AreEqual(18.75, lottery.getPrizeAmountByPlace(2));
        Assert::AreEqual(12.5, lottery.getPrizeAmountByPlace(3));
      }
    }
    TEST_METHOD(LotteryDraw_NegativeTests)
    {
      string errMsg;
      std::unique_ptr<CLotteryI> pLottery(LotteryObj_Create());
      CLotteryI &lottery = *pLottery;
      size_t iBallNumb = 0;
      Assert::IsTrue(lottery.purchase("Egor", iBallNumb));
      Assert::IsTrue(1 == iBallNumb);
      Assert::IsTrue(lottery.purchase("Andre", iBallNumb));
      Assert::IsTrue(2 == iBallNumb);
      {// Negative Test: 
        Assert::IsFalse(lottery.draw());
        errMsg = lottery.getLastError();
        Assert::IsFalse(errMsg.empty());
        Assert::IsTrue(errMsg.compare("Not enough participants (2) to select 3 winner(s)") == 0);
      }
      {// Negative Test: validate error handling of the same purchaser
        Assert::IsFalse(lottery.purchase("Egor", iBallNumb));
        errMsg = lottery.getLastError();
        Assert::IsFalse(errMsg.empty());
        Assert::IsTrue(errMsg.compare("Name 'Egor' already bought ticket.") == 0);
      }
      {// PrizeFunds
        Assert::IsFalse(lottery.addFunds(-250));
        errMsg = lottery.getLastError();
        Assert::IsFalse(errMsg.empty());
        Assert::IsTrue(errMsg.compare("No sufficient fund") == 0);
      }
      {// repeated Draw
        Assert::IsTrue(lottery.purchase("Lisa", iBallNumb));
        Assert::IsTrue(3 == iBallNumb);
        Assert::IsTrue(lottery.draw());
        Assert::IsFalse(lottery.draw());
      }
    }
    TEST_METHOD(LotteryDraw_ManyParticipants)
    {
      // 10'000 participants
      //#define PARTICIPANTS_CNT 3000
      #define PARTICIPANTS_CNT 10000
      //#define PARTICIPANTS_CNT 100000
      std::unique_ptr<CLotteryI> pLottery(LotteryObj_Create());
      CLotteryI &lottery = *pLottery;
      string sName;
      size_t iBallNumb = 0;
      for (size_t i = 0; i < PARTICIPANTS_CNT; i++)
      {
        sName= "Name"+to_string(i + 1);
        Assert::IsTrue(lottery.purchase(sName, iBallNumb));
        Assert::IsTrue(iBallNumb == i+1);
      }
      Assert::IsTrue(lottery.draw());
    }

    // check PrizeFund
    TEST_METHOD(PrizeFunds)
		{
      std::unique_ptr<CLotteryI> pLottery(LotteryObj_Create());
      CLotteryI &lottery = *pLottery;
      Assert::AreEqual(200.0, lottery.getPrizeFund(), ACCURACY);
      Assert::IsTrue(lottery.addFunds(50));
      Assert::AreEqual(250.0, lottery.getPrizeFund(), ACCURACY);
      Assert::IsTrue(lottery.addFunds(-50));
      Assert::AreEqual(200.0, lottery.getPrizeFund(), ACCURACY);
      Assert::IsTrue(lottery.addFunds(-0.001));
      Assert::AreEqual(199.999, lottery.getPrizeFund(), ACCURACY);
      Assert::IsTrue(lottery.addFunds(0.001));
      Assert::AreEqual(200.0, lottery.getPrizeFund(), ACCURACY);
    }

	};
}