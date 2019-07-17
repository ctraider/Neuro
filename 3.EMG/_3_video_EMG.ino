/*
EMG 
Оцифровывает сигнал c сенсора электромиограммы (electromyogram - EMG), подключенного ко входу А0 платы Arduino 
и отправляет для отображения в программу визуализации от BiTronics, скачать ее можно тут: http://www.bitronicslab.com/guide/ 
Справочник по командам языка Arduino: http://arduino.ru/Reference 
*/

#include <TimerOne.h> // подключаем библиотеку TimerOne для задействования функций Таймера1 
/* предварительно данную библиотеку надо установить, для чего скачиваем ее 
на странице http://www.bitronicslab.com/guide/ , распаковываем архив и помещаем папку TimerOne
внутрь папки libraries, находящейся тут: "Мои документы/Arduino/libraries" 
Подробнее о TimerOne см. тут: http://robocraft.ru/blog/arduino/614.html */

int val = 0;                                 // переменная для хранения оцифрованного значения сигнала  
int prev;
int prew;
int t;
int trig;
int rr;
int data;

// функция sendData вызывается каждый раз, когда срабатывает прерывание Таймера1 (проходит заданное число микросекунд)
void sendData() {
  Serial.write("A0");                       // записываем в Serial-порт имя поля в программе для визуализации, куда надо выводить сигнал
                                            // всего в этой программе 4 поля, которые имеют имена A0, A1, A2, A3 (сверху вниз, по порядку их 
                                            // расположения в окне программы)
  val = analogRead(A6);                     // записываем в переменную val оцифрованное значение сигнала с ножки А0 на Arduino.
                                            // val может принимать значение в диапазоне от 0 до 1023, см. http://arduino.ru/Reference/AnalogRead 
  Serial.write(map(val, 0, 1023, 0, 255));  // записываем в Serial-порт значение val, предварительно отнормированное на диапазон значений от 0 до 255,
                                            // см. описание команды map:  http://arduino.ru/Reference/Map 
  }

// функция setup вызывается однократно при запуске Arduino
void setup() {
  Serial.begin(115200);
    pinMode(13, OUTPUT);
    t = millis();
    data = analogRead(A6);
                     // инициализируем Serial-порт на скорости 115200 Кбит/c. 
                                           // такую же скорость надо установить в программе для визуализации
  Timer1.initialize(3000);                 // инициализируем Таймер1, аргументом указываем интервал срабатывания - 3000 микросекунд 
                                           // (1 000 000 микросекунд = 1 сек)
  Timer1.attachInterrupt(sendData);        // как только проходит 3000 микросекунд - наступает прерывание (вызывается функция sendData)
}

void loop(){
  Serial.write("A0");
  Serial.write(analogRead (A6) / 4);
  prew = data;
  data = analogRead(A6);
  if ((data>trig) && (prew<trig))
  {
    prew = t;
    t = millis();
    rr = t - prew;
    Serial.println (rr);
  }
  delay(3);

  }
