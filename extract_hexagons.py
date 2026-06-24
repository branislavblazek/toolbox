#!/usr/bin/env python3
"""Walk a folder, extract every ZIP, take the single CSV from each and build:
  - hexagons_unique.csv     -> unique hex_id (first occurrence, all columns)
  - hexagons_duplicates.csv -> hex_id found in more than one CSV (duplicates)
"""

from __future__ import annotations

import csv
import sys
import zipfile
from pathlib import Path

SOURCE_DIR = Path("/Users/branislav/Desktop/OneDrive_1_6-24-2026")
HEX_ID_COLUMN = "hex_id"

UNIQUE_OUTPUT = SOURCE_DIR / "hexagons_unique.csv"
DUPLICATES_OUTPUT = SOURCE_DIR / "hexagons_duplicates.csv"


def find_single_csv(extract_dir: Path) -> Path | None:
    csv_files = list(extract_dir.rglob("*.csv"))
    if not csv_files:
        return None
    if len(csv_files) > 1:
        print(f"    ! Found {len(csv_files)} CSV files, using the first one.")
    return csv_files[0]


def main() -> int:
    if not SOURCE_DIR.is_dir():
        print(f"ERROR: folder does not exist: {SOURCE_DIR}")
        return 1

    zip_files = sorted(SOURCE_DIR.glob("*.zip"))
    print(f"Processing folder: {SOURCE_DIR}")
    print(f"Found {len(zip_files)} ZIP files.\n")

    if not zip_files:
        print("No ZIP files to process. Done.")
        return 0

    header: list[str] | None = None
    hex_idx: int = 0

    unique_rows: dict[str, list[str]] = {}
    duplicates: list[tuple[str, str]] = []

    total_rows = 0

    for i, zip_path in enumerate(zip_files, start=1):
        print(f"[{i}/{len(zip_files)}] Extracting: {zip_path.name}")
        extract_dir = zip_path.parent / f"{zip_path.stem}__extracted"

        try:
            with zipfile.ZipFile(zip_path) as zf:
                zf.extractall(extract_dir)
        except zipfile.BadZipFile:
            print(f"    ! Corrupt ZIP, skipping: {zip_path.name}")
            continue

        csv_path = find_single_csv(extract_dir)
        if csv_path is None:
            print("    ! No CSV in this ZIP, skipping.")
            continue

        print(f"    Reading CSV: {csv_path.name}")
        with csv_path.open(newline="", encoding="utf-8") as f:
            reader = csv.reader(f)
            try:
                file_header = next(reader)
            except StopIteration:
                print("    ! Empty CSV, skipping.")
                continue

            if header is None:
                header = file_header
                if HEX_ID_COLUMN in header:
                    hex_idx = header.index(HEX_ID_COLUMN)
                else:
                    hex_idx = 0
                    print(
                        f"    ! Column '{HEX_ID_COLUMN}' not found, "
                        f"using first column '{header[0]}'."
                    )

            file_rows = 0
            file_dupes = 0
            for row in reader:
                if not row:
                    continue
                hex_id = row[hex_idx]
                total_rows += 1
                file_rows += 1
                if hex_id in unique_rows:
                    duplicates.append((hex_id, zip_path.name))
                    file_dupes += 1
                else:
                    unique_rows[hex_id] = row

            print(f"    Rows: {file_rows}, duplicates: {file_dupes}")

    if header is None:
        print("\nNo usable CSV data found. Done.")
        return 1

    print(f"\nWriting unique hexagons -> {UNIQUE_OUTPUT}")
    with UNIQUE_OUTPUT.open("w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(header)
        writer.writerows(unique_rows.values())

    print(f"Writing duplicates -> {DUPLICATES_OUTPUT}")
    with DUPLICATES_OUTPUT.open("w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow([header[hex_idx], "source_zip"])
        writer.writerows(duplicates)

    distinct_dupe_ids = len({hex_id for hex_id, _ in duplicates})

    print("\n" + "-" * 50)
    print(" DONE")
    print("-" * 50)
    print(f"Total rows processed:      {total_rows}")
    print(f"Unique hexagons:           {len(unique_rows)}")
    print(f"Duplicate occurrences:     {len(duplicates)}")
    print(f"Distinct duplicate hex_id: {distinct_dupe_ids}")
    print("-" * 50)
    return 0


if __name__ == "__main__":
    sys.exit(main())
