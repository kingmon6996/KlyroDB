import argparse
import random
import uuid

def generate_csv(rows, filename):
    with open(filename, 'w') as f:
        f.write("id,name,age,email,is_active\n")
        for i in range(1, rows + 1):
            name = f"User_{i}_{random.randint(100, 999)}"
            age = random.randint(18, 90)
            email = f"user{i}@example.com"
            active = random.choice(["TRUE", "FALSE"])
            f.write(f"{i},{name},{age},{email},{active}\n")
            
if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--rows", type=int, default=1000)
    parser.add_argument("--out", type=str, default="dataset.csv")
    args = parser.parse_args()
    generate_csv(args.rows, args.out)
    print(f"Generated {args.rows} rows to {args.out}")
