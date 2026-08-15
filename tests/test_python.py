import sys
import os

# Add local path to test before building wheel
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../python')))

try:
    import klyro
    print("klyro package imported successfully")
except ImportError as e:
    print(f"klyro package import failed: {e}")
