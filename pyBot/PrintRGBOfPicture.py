import numpy as np
from PIL import Image 
import sys


picture = np.array(Image.open(sys.argv[1]), dtype=float)
print(picture)