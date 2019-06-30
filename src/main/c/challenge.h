#ifndef CHALLENGE_H
#define CHALLENGE_H

/*
 * How sure should we be that a number is prime?
 */

#define NUM_PRIME_REPS 40

/*
 * Define the challenge parameters
 */

#if !(_RUN_FULL || _RUN_FAKE || _RUN_EXAMPLE)
#define _RUN_EXAMPLE 1
#endif

#if _RUN_EXAMPLE

#define N "253"
#define Z "19"
#define T ((uint64_t) 10UL)

#define P "11"
#define Q "23"

#elif _RUN_FAKE

#define N \
  "101719652584022021721762781591987403106802240366075660108083" \
  "367704090778501601086734883068039532343709625138045501658217" \
  "709188614695624121730133399703740726322970793554788655366823" \
  "409285170585009316076689680228051795942989207860731905045569" \
  "803739626866471393550678926733071101526714579757990084094647" \
  "654061965802946062714867235327401895309415994419500918525242" \
  "723085235188985250654159932125566140303155669511063926771219" \
  "582955695217106514196617688931760020855522615249054907822589" \
  "796844251497718961805379770736716138279096368086076000456821" \
  "443658984351114806981329739823066287119347189634564073063722" \
  "201314322083623"

#define Z \
  "921622794869691590767806801609427304612199313613403929258171" \
  "692315398184584573615428269534820651597434388193208351242126" \
  "295365918909179887341302564165026814881908823194821322499912" \
  "952879130044457275470053186665232839055473766122557756207644" \
  "308396380808683077786295459625413505828697285902321650472946" \
  "766220470173514020222563661224727603695356319696944229667989" \
  "724407714884184572017696832841210779360507908359489138426916" \
  "699411263543117481763781149942586361247375434963986361381762" \
  "754013335512102413924596259113284479173223424706989876294526" \
  "692369894869107426803861518411259964070442140608157227154157" \
  "884236247348"

#define T ((uint64_t) 31536000UL)

#define P \
  "760061255038074767615950132587144874505518927112391485295391" \
  "261928656185897103735791679401196143519586449861724318444011" \
  "657172789801679097300741079651091341954609168916795514742900" \
  "795473489248494151688178481976776575054434398984163984720364" \
  "969054342264390449528693624131592690455931267046593808523893" \
  "6670059"

#define Q \
  "133830861538819582072802576471866493004610474240700908837737" \
  "669570018042681249461345471541018313748792300132243220186774" \
  "913139101766321193189404911925737683724412887392371859118273" \
  "677434346223405790575197356914343464632753193230452143616075" \
  "841782379167774317257971573760079516631765547587282453114398" \
  "49798197"

#else

#define N \
  "631446608307288889379935712613129233236329881833084137558899" \
  "077270195712892488554730844605575320651361834662884894808866" \
  "350036848039658817136198766052189726781016228055747539383830" \
  "826175971321892666861177695452639157012069093997368008972127" \
  "446466642331918780683055206795125307008202024124623398241073" \
  "775370512734449416950118097524189066796385875485631980550727" \
  "370990439711973361466670154390536015254337398252457931357531" \
  "765364633198906465140213398526580034199190398219284471021246" \
  "488745938885358207031808428902320971090703239693491996277899" \
  "532332018406452247646396635593736700936921275809208629319872" \
  "7008292431243681"

#define Z \
  "427338526681239414707099486152541907807623930474842759553127" \
  "699575212802021361367225451651600353733949495680760238284875" \
  "258690199022379638588291839885522498545851997481849074579523" \
  "880422628363751913235562086585480775061024927773968205036369" \
  "669785002263076319003533000450157772067087172252728016627835" \
  "400463807389033342175518988780339070669313124967596962087173" \
  "533318107116757443584187074039849389081123568362582652760250" \
  "029401090870231288509578454981440888629750522601069337564316" \
  "940360631375375394366442662022050529545706707758321979377282" \
  "989361374561414204719371297211725179287931039547753581030226" \
  "7611143659071382"

#define T ((uint64_t) 79685186856218UL)

#define P \
  "154562048657421937084682913138460711844778329290535727168950" \
  "717733424202505601392625877205195325207546299861196433571846" \
  "578805282932208177729566895723130005216848832881503977502006" \
  "366130238044889916634521661721389848859842071584845041109033" \
  "358607085246937424599502955074736164230571310927294787034883" \
  "784001451"

#define Q \
  "408539233137919050563900062303270289825966069453463188465493" \
  "376573793671892840517393936233981207739676664433514053997334" \
  "707812632582103283756979861822638653429524314304519311998802" \
  "621852161840222046448205222543045576400382205048119934265113" \
  "402969976275851295274268069954115783249374069761252390893839" \
  "93333731"

#endif

#endif