#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi网络名称（SSID）和密码
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// 定义温度传感器的数据引脚
#define SENSOR_PIN_1 19
#define SENSOR_PIN_2 20

// 定义风扇控制引脚
#define FAN_PIN_1 21
#define FAN_PIN_2 31

// 创建OneWire实例用于与DS18B20通信
OneWire oneWire1(SENSOR_PIN_1);
OneWire oneWire2(SENSOR_PIN_2);

// 传递我们的OneWire引用到Dallas Temperature.
DallasTemperature sensors1(&oneWire1);
DallasTemperature sensors2(&oneWire2);

void setup(void)
{
  // 初始化串口，用于调试
  Serial.begin(115200);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // 启动WiFi连接
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // 启动温度传感器库
  sensors1.begin();
  sensors2.begin();

  // 设置风扇控制引脚为输出
  pinMode(FAN_PIN_1, OUTPUT);
  pinMode(FAN_PIN_2, OUTPUT);
}

void loop(void)
{
  // 请求所有传感器的数据
  sensors1.requestTemperatures();
  sensors2.requestTemperatures();

  // 获取并打印传感器的温度值
  float tempC1 = sensors1.getTempCByIndex(0); // 第一个传感器
  float tempC2 = sensors2.getTempCByIndex(0); // 第二个传感器

  Serial.print("Sensor 1: ");
  Serial.print(tempC1);
  Serial.println(" °C");

  Serial.print("Sensor 2: ");
  Serial.print(tempC2);
  Serial.println(" °C");

  // 计算平均温度
  float avgTemp = (tempC1 + tempC2) / 2;

  // 打印平均温度
  Serial.print("Average Temperature: ");
  Serial.print(avgTemp);
  Serial.println(" °C");

  // 如果平均温度超过25度，启动风扇
  if (avgTemp > 25.0)
  {
    digitalWrite(FAN_PIN_1, HIGH);
    digitalWrite(FAN_PIN_2, HIGH);
    Serial.println("Fan ON");
  }
  else
  {
    digitalWrite(FAN_PIN_1, LOW);
    digitalWrite(FAN_PIN_2, LOW);
    Serial.println("Fan OFF");
  }

  // 等待1秒钟后重复
  delay(1000);
}
