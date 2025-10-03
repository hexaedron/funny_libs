import math

rt = 100000;
r  = 100000;

print("const int16_t NTC_100Kx10 [] = {")
for i in range(1, 4095):
    res = 10*(1.0 / ((math.log(r / rt / (4095.0 / i - 1.0)) / 3950) + 1.0 / (25 + 273.15)) - 273.15)
    res = round(res)
    print ('/*', i, '*/', res)
print ("}")
