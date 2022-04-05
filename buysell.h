#include "XianDuan.h"
#include "ZhongShu.h"
#include "BaoHanChuLi.h"
#include <vector>


using namespace std;

enum class Period {OneMin, FiveMin, ThirtyMin, Day, Week, Month};
enum class BuySellType {None, OneBuy, FailureOneBuy, TwoBuy, SecondTwoBuy, ThreeBuy, OneSell, TwoSell, SecondTwoSell, ThreeSell};
class BuySell {
    private:
        BuySellType type;
        float price;
        Period period;
        int pos;
        vector <XianDuan>xd_list;

    public:
        BuySell() {
            this->type = BuySellType::None;
            this->price = 0.0;
        }

        BuySell(BuySellType type, float price, int pos, Period period ) {
            this->type = type;
            this->price = price;
            this->pos = pos;
            this->period = period;
        }

        BuySellType get_type() {
            return(this->type);
        }

        int get_pos() {
            return(this->pos);
        }
};


//enum class BuySellChuLiStatus {Start, OneBuy, TwoBuy, SecondTwoBuy, ThreeBuy, OneSell, TwoSell, SecondTwoSell, ThreeSell};
enum class BuySellChuLiStatus {Start, UpOne, DownOne, UpTwo, DownTwo, UpThree, UpFour, UpFour_Normal, A, UpThree_HighLow, UpThree_normal, DownThree, UpFour, DownFourNormal, DownFour};
class BuySellChuLi {
    private:
        BuySellChuLiStatus status;
        void debug_buysell(XianDuan xd);
        BuySell find_buysell(XianDuan xd, Period pd);
        XianDuanChuLi xdcl;
        ZhongShuChuLi zscl;
        ZhongShu A, B;
        XianDuan one, two, three, four, five, six, sevent;
    public:
        BuySellChuLi();
        void handle(vector<Kxian1>& kxlist);

        vector<BuySell> buysell_List;
        vector<BuySell> key_buysell_List;
};

void Bi3_buysell(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi4_buysell(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
extern void OutputDebugPrintf(const char* strOutputStrinng, ...);