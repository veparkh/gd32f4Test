## Опрос 2 NTC датчиков и отправка по CAN  на FreeRTOS для GD32F450
 Протокол CAN (ID 0x100):
  ```
  [0-1] NTC1 температура, int16 LE, десятые °C
  [2]   NTC1 valid
  [3-4] NTC2 температура, int16 LE, десятые °C
  [5]   NTC2 valid
  [6-7] 0
  ```

  Порог насыщения АЦП 4000 так как стабилитрон может портить измерения около 3.3 В.

  ### Как собрать
  ```
  cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-gcc-toolchain.cmake
  cmake --build build
  ```
