# Návod: Manuálna úprava metadát vo Word (.docx)

Tento dokument popisuje proces rozbalenia Word dokumentu, úpravu jeho interných XML metadát a následné zabalenie späť. Tento postup je užitočný, ak potrebujete zmeniť informácie o aplikácii alebo verzii, ktoré Word bežne neumožňuje editovať cez rozhranie.

## 1. Použité príkazy (Terminál)

Tieto príkazy vykonajte v priečinku, kde sa nachádza váš dokument:

```bash
# 1. Vytvorenie kópie pre prácu
cp "subor.docx" "subor_update.docx"

# 2. Rozbalenie štruktúry dokumentu
unzip "subor_update.docx" -d "temp_word"

# 3. Úprava metadát (otvorí textový editor)
nano "temp_word/docProps/app.xml"

# 4. Presun do dočasného priečinka
cd "temp_word"

# 5. Zabalenie obsahu späť do .docx (ignoruje systémové súbory)
zip -r ../"Hotovo.docx" * -x "*.DS_Store"

# 6. Návrat a vymazanie dočasných súborov
cd ..
rm -rf "temp_word" "subor_update.docx
```
