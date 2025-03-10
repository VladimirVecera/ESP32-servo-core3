# ESP32-servo-core3

Níže naleznete ukázku univerzální knihovny pro ovládání serva pomocí nového LEDC API na ESP32 (CORE 3) – knihovna je napsána tak, aby bylo možné měnit nastavení (například pin, kanál, timer, frekvenci, rozlišení, minimální a maximální pulz). Navíc je implementována neblokující verze, tedy bez použití delay, kde se plynulý přechod serva (směrem k cílovému úhlu) aktualizuje v metodě update(), kterou voláte pravidelně z loop().

Níže najdete tři soubory:

MyServo.h – hlavičkový soubor s deklarací třídy
MyServo.cpp – implementace třídy
Servo.ino – příklad, jak knihovnu používat (neblokující změna cílového úhlu pomocí millis)
