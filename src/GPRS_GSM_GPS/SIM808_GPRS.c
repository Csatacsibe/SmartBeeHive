/*
 * SIM808_GPRS.c
 *
 *  Created on: 2016. szept. 8.
 *      Author: Bence
 */

#include <GPRS_GSM_GPS/SIM808_GPRS.h>
#include <GPRS_GSM_GPS/SIM808_driver.h>
#include <GPRS_GSM_GPS/SIM808_device.h>
#include <device_management.h>

#include "string.h"

typedef enum bearer_status
{
  Connecting = 0,
  Connected  = 1,
  Closing    = 2,
  Closed     = 3
}bearer_status_t;


typedef struct gprs_info
{
  uint8_t cid;
  char ip_address[16];
  bearer_status_t bearer_status;

  uint8_t http_status_code;
}gprs_info_t;

static boolean_t ok = False;
static gprs_info_t gprs_info = {0};

/* response callbacks */
static void process_CGATT_resp(char* data);
static void process_OK_resp(char* data);
static void process_CSQ_resp(char* data);
static void process_SAPBR_resp(char* data);
static void process_HTTPACTION_resp(char* data);

/*                    */
static boolean_t cmd_SAPBR(uint8_t cmd_type, uint8_t cid, char* ConParamTag, char* ConParamValue, uint32_t timeout);
static boolean_t get_status_and_IP(uint32_t timeout);
static boolean_t check_ip(void);
static boolean_t cmd_HTTPPARA(char* HttpParamTag, char* HttpParamValue, uint32_t timeout);
static boolean_t is_GPRS_attached(void);
static boolean_t attach_GPRS(boolean_t enable);
static boolean_t configure_bearer(uint32_t timeout);
static boolean_t configure_HTTP(uint32_t timeout);

void init_GPRS()
{
  gprs_info.cid = 1;
  gprs_info.bearer_status = Closed;
  strncpy(gprs_info.ip_address, "0.0.0.0", 8);
}

boolean_t check_sig_strength_SIM808()
{
  put_s_SIM808("AT+CSQ\r");
  get_s_SIM808(4, process_CSQ_resp);

  return waitFor(&ok, 1000);
}

boolean_t connect_GPRS(uint32_t timeout)
{
  boolean_t ret = False;

  ret = is_SIM_registered_SIM808();
  if(False == ret) return ret;

  ret = attach_GPRS(True);
  if(False == ret) return ret;

  ret = is_GPRS_attached();
  if(False == ret) return ret;

  ret = check_sig_strength_SIM808();
  if(False == ret) return ret;

  ret = configure_bearer(timeout);
  if(False == ret) return ret;

  ret = configure_HTTP(timeout);
  if(False == ret) return ret;

  return ret;
}

boolean_t disconnect_GPRS(uint32_t timeout)
{
  boolean_t ret = False;

  put_s_SIM808("AT+HTTPTERM\r");
  get_s_SIM808(2, process_OK_resp);

  if(False == (ret = waitFor(&ok, timeout)))
    return ret;

  ret = cmd_SAPBR(0, gprs_info.cid, NULL, NULL, timeout);
  if(!ret) return ret;

  ret = attach_GPRS(False);
  if(False == ret) return ret;

  return ret;
}

boolean_t download_data_GPRS(char* data, uint32_t timeout)
{
  char cmd[50] = {0};
  boolean_t ret = False;

  snprintf(cmd, 49, "AT+HTTPDATA=%d,%d\r", length(data), (int)timeout);
  put_s_SIM808(cmd);

  while('D' != get_c_SIM808()); // TODO: timeout

  put_s_SIM808(data);
  get_s_SIM808(2, process_OK_resp);

  if(False == (ret = waitFor(&ok, timeout)))
    return ret;

  // POST method
  gprs_info.http_status_code = 0;

  put_s_SIM808("AT+HTTPACTION=1\r");
  get_s_SIM808(4, process_HTTPACTION_resp);

  if(False == (ret = waitFor(&ok, timeout)))
    return ret;

  //read response to catch with analyzer
  put_s_SIM808("AT+HTTPREAD\r");

  return ret;
}

static void process_HTTPACTION_resp(char* data)
{
  if(False == check_msg_header_SIM808(data, "+HTTPACTION"))
  {
    ok = False;
    return;
  }

  while(data[0] != ',')
  {
    data++;
  }
  data++;

  gprs_info.http_status_code = (data[0] - 48) * 100 + (data[1] - 48) * 10 + data[2] - 48;

  if(gprs_info.http_status_code == 200)
    ok = True;
  else
    ok = False;
}

static boolean_t configure_HTTP(uint32_t timeout)
{
  boolean_t ret = False;

  put_s_SIM808("AT+HTTPINIT\r");
  get_s_SIM808(2, process_OK_resp);

  if(False == (ret = waitFor(&ok, timeout)))
    return ret;

  // Set HTTP profile identifier
  ret = cmd_HTTPPARA("CID", NULL, timeout);
  if(!ret) return ret;

  // Put in the URL of the PHP webpage where you will post -
  // the URL below is a test server so you can use it in testing
  ret = cmd_HTTPPARA("URL", "http://posttestserver.com/post.php", timeout);
  if(!ret) return ret;

  return ret;
}

static boolean_t configure_bearer(uint32_t timeout)
{
  boolean_t ret = False;

  // Set connection type to GPRS
  ret = cmd_SAPBR(3, gprs_info.cid, "Contype", "GPRS", timeout);
  if(!ret) return ret;

  // Set the APN - this will depend on your network/service provider
  ret = cmd_SAPBR(3, gprs_info.cid, "APN", "wholesale", timeout);
  if(!ret) return ret;

  // Enable GPRS - this will take a moment or two
  ret = cmd_SAPBR(1, gprs_info.cid, NULL, NULL, timeout);
  if(!ret) return ret;

  ret = get_status_and_IP(timeout);
  if(!ret) return ret;

  return ret;
}

static boolean_t attach_GPRS(boolean_t enable)
{
  if(enable == True)
  {
    put_s_SIM808("AT+CGATT=1\r");
  }
  else
  {
    put_s_SIM808("AT+CGATT=0\r");
  }

  get_s_SIM808(2, process_OK_resp);

  return waitFor(&ok, 1000);
}

static boolean_t is_GPRS_attached()
{
  put_s_SIM808("AT+CGATT?\r");
  get_s_SIM808(4, process_CGATT_resp);

  return waitFor(&ok, 1000);
}

static boolean_t cmd_HTTPPARA(char* HttpParamTag, char* HttpParamValue, uint32_t timeout)
{
  char cmd[200] = {0};

  if(NULL == HttpParamValue)
    snprintf(cmd, 199, "AT+HTTPPARA=\"%s\",%d\r", HttpParamTag, gprs_info.cid);
  else
    snprintf(cmd, 199, "AT+HTTPPARA=\"%s\",\"%s\"\r", HttpParamTag, HttpParamValue);

  put_s_SIM808(cmd);
  get_s_SIM808(2, process_OK_resp);

  return waitFor(&ok, timeout);
}

static boolean_t get_status_and_IP(uint32_t timeout)
{
  char cmd[35] = {0};
  snprintf(cmd, 34, "AT+SAPBR=2,%d\r", gprs_info.cid);

  put_s_SIM808(cmd);
  get_s_SIM808(4, process_SAPBR_resp);

  return waitFor(&ok, timeout);
}

static void process_SAPBR_resp(char* data)
{
  if(False == check_msg_header_SIM808(data, "+SAPBR"))
  {
    ok = False;
    return;
  }

  uint8_t length = 0;

  while(data[0] != ',')
  {
    data++;
  }
  data++;

  gprs_info.bearer_status = data[0] - 48;

  while(data[0] != '"')
  {
    data++;
  }
  data++;

  for(length = 0; data[length] != '"'; length++);

  strncpy(gprs_info.ip_address, data, length);

  if(check_ip())
    ok = True;
  else
    ok = False;
}

static boolean_t check_ip()
{
  if(0 == strncmp(gprs_info.ip_address, "0.0.0.0", 8))
    return False;
  else
    return True;
}

static boolean_t cmd_SAPBR(uint8_t cmd_type, uint8_t cid, char* ConParamTag, char* ConParamValue, uint32_t timeout)
{
  char cmd[35] = {0};

  if(NULL == ConParamTag)
    snprintf(cmd, 34, "AT+SAPBR=%d,%d\r", cmd_type, cid);
  else
    snprintf(cmd, 34, "AT+SAPBR=%d,%d,\"%s\",\"%s\"\r", cmd_type, cid, ConParamTag, ConParamValue);

  put_s_SIM808(cmd);
  get_s_SIM808(2, process_OK_resp);

  return waitFor(&ok, timeout);
}

static void process_OK_resp(char* data)
{
  data++; // suppress warning
  ok = True;
}

static void process_CSQ_resp(char* data)
{
  if(check_msg_header_SIM808(data, "+CSQ") == False)
  {
    ok = False;
    return;
  }

  uint8_t value = 0;

  while(data[0] != ',')
  {
    data++;
  }
  data--;

  value = data[0] - 48;
  data--;

  if(data[0] != ' ')
    value += 10 * (data[0] - 48);

  if(value > 8 && value != 99)
  {
    ok = True;
  }
  else
  {
    ok = False;
  }
}

static void process_CGATT_resp(char* data)
{
  if(check_msg_header_SIM808(data, "+CGATT") == False)
  {
    ok = False;
    return;
  }

  while(data[0] != ' ')
  {
    data++;
  }
  data++;

  if(data[0] - 48 == 1)
  {
    ok = True;
  }
  else
  {
    ok = False;
  }
}
