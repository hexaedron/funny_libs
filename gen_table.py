import math

rt = 100000;
r  = 2500;

for i in range(1, 1023):
    res = (1.0 / ((math.log(r / rt / (1023.0 / i - 1.0)) / 3950) + 1.0 / (25 + 273.15)) - 273.15)
    res = round(res)
    print (i,';',res)
