/*
EMG_LED
Оцифровывает аналоговый сигнал со входа А0 платы Arduino. По превышению порогового уровня - включает светодиод.
Считанный сигнал также отображается в программе визуализации от BiTronics, скачать ее можно тут: http://www.bitronicslab.com/guide/ 
Справочник по командам языка Arduino: http://arduino.ru/Reference 
*/

#include <TimerOne.h> // подключаем библиотеку TimerOne для задействования функций Таймера1 
/* предварительно данную библиотеку надо установить, для чего скачиваем ее 
на странице http://www.bitronicslab.com/guide/ , распаковываем архив и помещаем папку TimerOne
внутрь папки libraries, находящейся тут: "Мои документы/Arduino/libraries" 
Подробнее о TimerOne см. тут: http://robocraft.ru/blog/arduino/614.html */

#define arrSize 32                           // размер массива оцифрованных значений для поиска в нем максимума и минимума сигнала
#define threshold 40                         // порог срабатывания светодиода (может требовать подстройки под конкретного человека)
#define LED 13                               // определим номер вывода Arduino, к которому подключается светодиод
double sData = 0;                            // амплитуда размаха ЭМГ-сигнала
int val[arrSize];                            // массив для хранения оцифрованных значений ЭМГ-сигнала
int i = 0;                                   // переменная-счетчик 
int emg = 0;                                 // переменная для хранения оцифрованного значения сигнала  
int maxV = 0, minV = 0;                      // переменные для хранения максимального и минимального значений ЭМГ-сигнала в массиве val[] 

// функция setup вызывается однократно при запуске Arduino
void setup() {
  Serial.begin(115200);                      // инициализируем Serial-порт на скорости 115200 Кбит/c. 
                                             // такую же скорость надо установить в программе для визуализации
  Timer1.initialize(3000);                   // инициализируем Таймер1, аргументом указываем интервал срабатывания - 3000 микросекунд 
                                             // (1 000 000 микросекунд = 1 сек)
  Timer1.attachInterrupt(sendData);          // как только проходит 3000 микросекунд - наступает прерывание (вызывается функция sendData)
  pinMode(LED, OUTPUT);                      // Определим вывод LED как выход, к нему подключим светодиод. См. описание  pinMode(): http://arduino.ru/Reference/PinMode
  digitalWrite(LED, LOW);                    // выключаем светодиод, см. описание digitalWrite(): http://arduino.ru/Reference/DigitalWrite 
 
 // заполняем все элементы массива val[] нулями
 for (int k = 0; k < arrSize; k++){
   val[k] = 0;
 }
}

void sendData() {
  Serial.write("A0");                         // записываем в Serial-порт имя поля в программе для визуализации, куда надо выводить сигнал
                                              // всего в этой программе 4 поля, которые имеют имена A0, A1, A2, A3 (сверху вниз, по порядку их 
                                              // расположения в окне программы)
  emg = analogRead(A0);                       // записываем в переменную val оцифрованное значение сигнала с ножки А0 на Arduino.
                                              // emg может принимать значение в диапазоне от 0 до 1023, см. http://arduino.ru/Reference/AnalogRead 
  val[i] = map(emg, 0, 1023, 0, 255);         // записываем в элемент массива val[i] переменную emg, предварительно отнормированную на диапазон значений от 0 до 255,
                                              // см. описание команды map:  http://arduino.ru/Reference/Map 
  Serial.write(val[i]);                       // отправляем результат оцифровки
  i++;                                        // инкрементируем (увеличиваем на 1) значение переменной-счетчика
  
  if (i == arrSize) {                         // если полностью заполнили массив
    i = 0;                                    // сбрасываем значение переменной-счетчика в 0
    
    // Ищем максимальное и минимальное значение ЭМГ-сигнала в массиве val[] 
    for (int k = 0; k < arrSize; k++) {       // создаем цикл for c переменной-итератором k и начинаем перебирать элементы массива val[]
      if (val[k] > maxV)                      // если элемент массива val[k] больше, чем значение ранее записанное в переменной для хранения максимумов maxV,
        maxV = val[k];                        // обновляем значение maxV
      if (val[k] < minV)                      // если же элемент массива val[k] меньше, чем значение ранее записанное в переменной для хранения минимумов minV,
        minV = val[k];                        // обновляем значение minV
    }
    sData =  0.4*sData + 0.6*(maxV - minV); // чтобы срабатывание мотора было более плавным, раскомментируйте эту строку и закомментируйте следующую. Теория: https://ru.wikipedia.org/wiki/Экспоненциальное_сглаживание 
   // sData = maxV - minV;                      // определяем максимальный размах сигнала ЭМГ, записанного в массив val[]
    maxV = 0;                                 // обнуляем значение переменной maxV, в которой хранится значение максимального элемента массива val[] 
    minV = 255;                                 // обнуляем значение переменной minV, в которой хранится значение минимального элемента массива val[] 
  }
   
 if (sData > threshold)                       // если размах сигнала больше порога срабатывания
    digitalWrite(LED, HIGH);                  // устанавливаем логическую "1" на выход LED (высокий уровень напряжения на выходе - светодиод включается)
 else
    digitalWrite(LED, LOW);                   // иначе устанавливаем логический "0" на выход LED (низкий уровень напряжения на выходе - светодиод выключается)
}

void loop() {
// в бесконечном цикле мы ничего не делаем. Таймер1 сам будет вызывать функцию sendData через каждые 3000 микросекунд
}

// упражнение: перепишите скетч так, чтобы в нем не нужно было использовать массив для хранения оцифрованных значений сигнала
