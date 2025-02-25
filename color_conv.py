import sys

r = int(sys.argv[1])
g = int(sys.argv[2])
b = int(sys.argv[3])
c = int(sys.argv[4])

print("RGB", 
      r / c * 255.0,
      g / c * 255.0,
      b / c * 255.0,
      )

