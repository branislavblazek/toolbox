subjects = [
    {"grade": "B", "credits": 5.0},
    {"grade": "A", "credits": 5.0},
    {"grade": "C", "credits": 5.0},
    {"grade": "C", "credits": 5.0},
    {"grade": "A", "credits": 5.0},
    {"grade": "B", "credits": 5.0},
    {"grade": "A", "credits": 1.0},
    {"grade": "A", "credits": 1.0},
    {"grade": None, "credits": 5.0},
]

ECTS_WEIGHTS = {
    "A": 1.0,
    "B": 1.5,
    "C": 2.0,
    "D": 2.5,
    "E": 3.0,
    "FX": 4.0
}

FX_WEIGHT = 4.0

# 1. Absolvované predmety
points_absolved = 0.0
credits_absolved = 0.0

# 2. Výpočet celkového priemeru (vrátane otvorených predmetov penalizovaných ako FX)
points_all = 0.0
credits_all = 0.0

for p in subjects:
    credits = p["credits"]
    grade = p["grade"]
    
    if grade is not None:
        # Predmet je uzavretý
        grade_weight = ECTS_WEIGHTS[grade]
        
        points_absolved += grade_weight * credits
        credits_absolved += credits
        
        points_all += grade_weight * credits
        credits_all += credits
    else:
        # Predmet je otvorený (zatiaľ bez známky)
        points_all += FX_WEIGHT * credits
        credits_all += credits

print("-" * 50)
print(" VÝSLEDKY ECTS PRIEMERU")
print("-" * 50)

if credits_absolved > 0:
    average_absolved = points_absolved / credits_absolved
    print(f"Čistý priemer (iba uzavreté predmety): {average_absolved:.2f}")
    print(f"  -> Počet kreditov: {credits_absolved}")
else:
    print("Žiadne predmety nie sú uzavreté.")

if credits_all > 0:
    average_all = points_all / credits_all
    print(f"Celkový vážený priemer semestra/roka: {average_all:.2f}")
    print(f"  -> Celkový počet zapísaných kreditov: {credits_all}")
print("-" * 50)
