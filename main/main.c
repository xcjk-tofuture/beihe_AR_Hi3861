/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>

#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"

#include "hal_bsp_wifi.h"
#include "wifi_device.h"
#include "hal_bsp_mqtt.h"
#include "st7789.h"
#include "cJSON.h"


void parse_json_data(void);
void all_init(void);
void task_init(void);

#define WIFI_SSID "xcjk"
#define WIFI_PAWD "66666666"

#define SERVER_IP_ADDR "154.44.26.157"
#define SERVER_IP_PORT 1883
#define MQTT_TOPIC_SUB "beihe/weChatApp/configData"
#define MQTT_TOPIC_PUB "pubTopic"
#define TASK_INIT_TIME 2 // s
#define LED_INTERVAL_TIME_US 300000
#define MQTT_RECV_TASK_TIME (200 * 1000) // us
#define LED_TEST_GPIO 12 // for hispark_pegasus
uint8_t recvBuff[200] = {0};
uint8_t *pbuff = recvBuff;
enum LedState {
    LED_ON = 0,
    LED_OFF,
    LED_SPARK,
};

enum LedState g_ledState = LED_SPARK;

int8_t mqtt_sub_payload_callback(unsigned char *topic, unsigned char *payload)
{
    //printf("[info] topic:[%s]    recv<== %s\r\n", topic, payload);
    strcpy(recvBuff, payload);
    parse_json_data();
    printf("preget: %s\r\n", recvBuff);
}

static void *LedTask(const char *arg)
{

    usleep(500 * 1000);
    lcdInit();
    usleep(200 * 1000);
    LCD_Fill(0,0,135,240,WHITE);
    (void)arg;
    while (1) {
        switch (g_ledState) {
            case LED_ON:
                IoTGpioSetOutputVal(LED_TEST_GPIO, 1);
                usleep(LED_INTERVAL_TIME_US);
                
                break;
            case LED_OFF:
                IoTGpioSetOutputVal(LED_TEST_GPIO, 0);
                usleep(LED_INTERVAL_TIME_US);
                break;
            case LED_SPARK:
                IoTGpioSetOutputVal(LED_TEST_GPIO, 0);
                usleep(LED_INTERVAL_TIME_US);
                IoTGpioSetOutputVal(LED_TEST_GPIO, 1);
                usleep(LED_INTERVAL_TIME_US);
                LCD_Fill(0,0,100,100,RED);
                break;
            default:
                usleep(LED_INTERVAL_TIME_US);
                break;
        }
        //MQTTClient_pub(MQTT_TOPIC_PUB, "hello world!!!", strlen("hello world!!!"));
    }

    return NULL;
}

static void *MQTTTASK(const char *arg)
{
    (void)arg;
    while (1) {
        MQTTClient_sub();
        usleep(MQTT_RECV_TASK_TIME);
    }
}



static void main(void)
{
    all_init();
    task_init();
}

void all_init()
{
    if (WiFi_connectHotspots(WIFI_SSID, WIFI_PAWD) != WIFI_SUCCESS) {
        printf("[error] WiFi_connectHotspots\r\n");
    }
    sleep(TASK_INIT_TIME);
       // 连接MQTT服务器
    if (MQTTClient_connectServer(SERVER_IP_ADDR, SERVER_IP_PORT) != 0) {
        printf("[error] MQTTClient_connectServer\r\n");
    } else {
        printf("[success] MQTTClient_connectServer\r\n");
    }
    sleep(TASK_INIT_TIME);
    

    // 初始化MQTT客户端
    if (MQTTClient_init("hi3861-AR-01", "beihe/HI3861_AR", "66666666") != 0) {
        printf("[error] MQTTClient_init\r\n");
    } else {
        printf("[success] MQTTClient_init\r\n");
    }
    p_MQTTClient_sub_callback = &mqtt_sub_payload_callback;
    sleep(TASK_INIT_TIME);
        // 订阅Topic
    if (MQTTClient_subscribe(MQTT_TOPIC_SUB) != 0) {
        printf("[error] MQTTClient_subscribe\r\n");
    } else {
        printf("[success] MQTTClient_subscribe\r\n");
    }





    IoTGpioInit(LED_TEST_GPIO);
    IoTGpioSetDir(LED_TEST_GPIO, IOT_GPIO_DIR_OUT);
}


void task_init()
{
    osThreadAttr_t ledtask;
    ledtask.name = "LedTask";
    ledtask.attr_bits = 0U;
    ledtask.cb_mem = NULL;
    ledtask.cb_size = 0U;
    ledtask.stack_mem = NULL;
    ledtask.stack_size = 512 * 4;
    ledtask.priority = 25;
    if (osThreadNew((osThreadFunc_t)LedTask, NULL, &ledtask) == NULL) {
        printf("[Led] Falied to create LedTask!\n");
    }

    // osThreadAttr_t mqtttask;
    // mqtttask.name = "MQTTTask";
    // mqtttask.attr_bits = 0U;
    // mqtttask.cb_mem = NULL;
    // mqtttask.cb_size = 0U;
    // mqtttask.stack_mem = NULL;
    // mqtttask.stack_size = 512 * 2;
    // mqtttask.priority = 25;
    // if (osThreadNew((osThreadFunc_t)MQTTTASK, NULL, &mqtttask) == NULL) {
    //     printf("[MQTT] Falied to create LedTask!\n");
    // }

}


void parse_json_data(void)
{
    cJSON *json_root = cJSON_Parse((const char *)recvBuff);

    if (json_root) {
        //printf("json_root");

        cJSON *json_text = cJSON_GetObjectItem(json_root, "center");
        printf("get: %s\r\n", json_text->valuestring);
        json_text = NULL;
    }
    cJSON_Delete(json_root);
    json_root = NULL;
}


SYS_RUN(main);
