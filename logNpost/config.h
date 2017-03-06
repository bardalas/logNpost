//#define DEBUG 1

/**************************************
  Serial Messages
**************************************/
#define MSG_OK "OK"
#define MSG_HELLO "Init"
#define MSG_ERROR "ERR"


/**************************************
  AT Messages
**************************************/
#define AT_NET_REG "AT+CREG?\r" //Network Registration
#define AT_SET_GPRS "AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r" ////set GPRS mode
#define AT_SET_APN "AT+SAPBR=3,1,\"APN\",\"%s\"\r" //set APN
#define AT_OPEN_BRR "AT+SAPBR=1,1\r" //Open bearer
#define AT_QUERY_BRR "AT+SAPBR=2,1\r" //Query bearer
#define AT_SEND_HTTP "AT+HTTPACTION=0\r"
#define AT_SET_HTTP_GET "AT+HTTPPARA=\"CID\",1\r"
#define AT_INIT_HTTP "AT+HTTPINIT\r"
#define AT_ECHO_OFF "ATE0\r"
#define AT_READ_HTTP "AT+HTTPREAD\r"
#define AT_SET_URL "AT+HTTPPARA=\"URL\",\"%s%s\"\r" //URL,CMD
#define AT_SET_URL_DATA "AT+HTTPPARA=\"URL\",\"%s%s" //URL,CMD


/**************************************
  Operational Configurations
**************************************/
#define SERVER_URL "http://smarthouse.gear.host/cmd.php?"
#define START_LOG_CMD "set=1&t=Init,&s=System Init"
#define MAX_CONN_TIME 10000 /* ms */
#define AVG_COUNT 1
#define MEMS_READ_INTERVAL_MS 1
#define RECORDS_AMOUNT 40
#define INTERVAL_MS (8000 / RECORDS_AMOUNT)
#define BUF_SIZE (RECORDS_AMOUNT * 5) + 150 //keep 350 bytes minimum 556Max
#define APN "internetg"
#define TIME_ZONE_FIX 20000 //


/**************************************
  Variable Declerations
**************************************/
//GPS data
float     timeStamp = 0;

uint32_t  time = -1;
uint32_t  date = -1;
uint32_t  lastTime = 0;
int       srvCMD;
uint8_t   speed = 1;
uint8_t   record = 0;
uint8_t   sat = 0 ;
uint8_t   speedChunk[RECORDS_AMOUNT];
char      buffer[BUF_SIZE];
bool      httpEnable = false;
bool      parseE = false;

#ifdef DEBUG
  bool      serialEditorEnable = false;
  String    serial;
#endif

/**************************************
  Function Declerations
**************************************/
void cloudLog(bool parseEnable = true, bool cloudEnable = false);

//GPRS
bool initGPRS();
//int getSignal();
//bool getOperatorName();
void sendHTTPGET(char *val);

//GPS
bool initGPS();
void showGPS();
bool proccessGPS();

//GSM
bool initGSM();
bool sendGSMCommand(const char* cmd, unsigned int timeout = 10, const char* expected = "OK");
void toggleGSM();




