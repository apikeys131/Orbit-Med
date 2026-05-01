// ============================================================================
//  Orbit-Med  v3.0  |  SDL2 + SDL2_image + SDL2_ttf  |  C++17
//  Features: animated atom logo, search, sort asc/desc, click-for-details
// ============================================================================
#define _USE_MATH_DEFINES
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

static constexpr int   WIN_W          = 1400;
static constexpr int   WIN_H          = 860;
static constexpr int   LOGO_CX        = 210;
static constexpr int   LOGO_CY        = 115;
static constexpr float RING_RX        = 88.0f;
static constexpr float RING_RY        = 28.0f;
static constexpr float RING_SPIN      = 0.0f;
static constexpr float ELEC_SPEED     = 1.3f;
static constexpr int   ELEC_R         = 9;
static constexpr int   NUCLEUS_R      = 18;
static constexpr float SHIMMER_SPEED  = 260.0f;
static constexpr int   SHIMMER_W      = 200;
static constexpr Uint8 SHIMMER_BRIGHT = 16;
static constexpr int   HEADER_H       = 230;
static constexpr int   SEARCH_Y       = HEADER_H + 12;
static constexpr int   SEARCH_H       = 34;
static constexpr int   HINT_Y         = SEARCH_Y + SEARCH_H + 6;
static constexpr int   TABLE_Y        = HINT_Y + 22;
static constexpr int   ROW_H          = 22;
static constexpr int   TABLE_X        = 16;
static constexpr int   SCROLLBAR_W    = 8;
static constexpr int   COL_IDX        = 44;
static constexpr int   COL_NAME       = 220;
static constexpr int   COL_CAT        = 135;
static constexpr int   COL_USE        = 310;
static constexpr int   COL_DOS        = 90;
static constexpr int   COL_PRI        = 72;

// Detail window
static constexpr int   DET_W          = 680;
static constexpr int   DET_H          = 580;

struct Medicine {
    const char* name;
    const char* category;
    const char* use;
    const char* dosage;
    float       price;
    const char* type;
};

static const Medicine MEDICINES[] = {
    { "Ibuprofen (Advil)", "Pain Relief", "Mild to moderate pain and fever", "200mg", 6.99f, "OTC" },
    { "Acetaminophen (Tylenol)", "Pain Relief", "Pain and fever reducer", "500mg", 7.49f, "OTC" },
    { "Naproxen (Aleve)", "Pain Relief", "Anti-inflammatory pain relief", "220mg", 9.99f, "OTC" },
    { "Aspirin (Bayer)", "Pain Relief", "Pain, fever, inflammation", "325mg", 5.49f, "OTC" },
    { "Celecoxib (Celebrex)", "Pain Relief", "COX-2 inhibitor for arthritis", "200mg", 45.00f, "Rx" },
    { "Tramadol (Ultram)", "Pain Relief", "Moderate to severe pain", "50mg", 32.00f, "Rx" },
    { "Oxycodone (OxyContin)", "Pain Relief", "Severe pain management", "10mg", 89.00f, "Rx" },
    { "Hydrocodone (Vicodin)", "Pain Relief", "Moderate to severe pain", "5mg", 67.00f, "Rx" },
    { "Morphine Sulfate (MS Contin)", "Pain Relief", "Severe chronic pain", "15mg", 112.00f, "Rx" },
    { "Codeine", "Pain Relief", "Mild to moderate pain", "30mg", 28.00f, "Rx" },
    { "Gabapentin (Neurontin)", "Pain Relief", "Neuropathic pain relief", "300mg", 22.00f, "Rx" },
    { "Pregabalin (Lyrica)", "Pain Relief", "Neuropathic and fibromyalgia pain", "75mg", 185.00f, "Rx" },
    { "Diclofenac (Voltaren)", "Pain Relief", "Inflammatory and arthritic pain", "50mg", 18.00f, "Rx" },
    { "Meloxicam (Mobic)", "Pain Relief", "Arthritis and joint pain", "7.5mg", 14.00f, "Rx" },
    { "Ketorolac (Toradol)", "Pain Relief", "Short-term moderate to severe pain", "10mg", 35.00f, "Rx" },
    { "Buprenorphine (Buprenex)", "Pain Relief", "Severe pain opioid partial agonist", "0.3mg", 145.00f, "Rx" },
    { "Fentanyl Patch (Duragesic)", "Pain Relief", "Chronic severe pain transdermal", "25mcg/hr", 210.00f, "Rx" },
    { "Indomethacin (Indocin)", "Pain Relief", "Severe inflammatory conditions", "25mg", 19.00f, "Rx" },
    { "Piroxicam (Feldene)", "Pain Relief", "Arthritis anti-inflammatory", "20mg", 23.00f, "Rx" },
    { "Butalbital/APAP/Caffeine", "Pain Relief", "Tension headache relief", "50/325/40mg", 29.00f, "Rx" },
    { "Lidocaine Patch (Lidoderm)", "Pain Relief", "Localized topical pain relief", "5% patch", 78.00f, "Rx" },
    { "Capsaicin Cream (Zostrix)", "Pain Relief", "Topical neuropathic pain relief", "0.025%", 11.99f, "OTC" },
    { "Methocarbamol (Robaxin)", "Pain Relief", "Musculoskeletal pain and spasm", "500mg", 26.00f, "Rx" },
    { "Tapentadol (Nucynta)", "Pain Relief", "Moderate to severe acute pain", "50mg", 155.00f, "Rx" },
    { "Ziconotide (Prialt)", "Pain Relief", "Intrathecal severe chronic pain", "25mcg/mL", 890.00f, "Rx" },
    { "Amoxicillin (Amoxil)", "Antibiotic", "Broad-spectrum bacterial infections", "500mg", 14.00f, "Rx" },
    { "Amoxicillin-Clavulanate (Augmentin)", "Antibiotic", "Beta-lactamase resistant infections", "875mg", 28.00f, "Rx" },
    { "Penicillin VK", "Antibiotic", "Strep and gram-positive infections", "500mg", 11.00f, "Rx" },
    { "Dicloxacillin", "Antibiotic", "Staphylococcal skin infections", "250mg", 18.00f, "Rx" },
    { "Nafcillin", "Antibiotic", "Staph aureus IV treatment", "1g", 95.00f, "Rx" },
    { "Piperacillin-Tazobactam (Zosyn)", "Antibiotic", "Broad-spectrum severe infections", "3.375g", 145.00f, "Rx" },
    { "Cephalexin (Keflex)", "Antibiotic", "Skin and UTI infections", "500mg", 16.00f, "Rx" },
    { "Cefazolin (Ancef)", "Antibiotic", "Surgical prophylaxis and infections", "1g", 42.00f, "Rx" },
    { "Cefuroxime (Ceftin)", "Antibiotic", "Respiratory and ear infections", "250mg", 22.00f, "Rx" },
    { "Cefdinir (Omnicef)", "Antibiotic", "Ear, sinus, and skin infections", "300mg", 34.00f, "Rx" },
    { "Ceftriaxone (Rocephin)", "Antibiotic", "Serious bacterial infections IV/IM", "1g", 78.00f, "Rx" },
    { "Cefepime (Maxipime)", "Antibiotic", "Gram-negative hospital infections", "1g", 112.00f, "Rx" },
    { "Ceftazidime (Fortaz)", "Antibiotic", "Pseudomonas and gram-negative inf.", "1g", 98.00f, "Rx" },
    { "Cefotaxime (Claforan)", "Antibiotic", "Meningitis and severe infections", "1g", 88.00f, "Rx" },
    { "Azithromycin (Zithromax)", "Antibiotic", "Respiratory and STI infections", "250mg", 19.00f, "Rx" },
    { "Clarithromycin (Biaxin)", "Antibiotic", "Upper respiratory infections", "500mg", 31.00f, "Rx" },
    { "Erythromycin (Ery-Tab)", "Antibiotic", "Penicillin-allergy alternative", "250mg", 24.00f, "Rx" },
    { "Ciprofloxacin (Cipro)", "Antibiotic", "UTI and gram-negative infections", "500mg", 17.00f, "Rx" },
    { "Levofloxacin (Levaquin)", "Antibiotic", "Respiratory and UTI infections", "500mg", 21.00f, "Rx" },
    { "Moxifloxacin (Avelox)", "Antibiotic", "Community-acquired pneumonia", "400mg", 38.00f, "Rx" },
    { "Doxycycline (Vibramycin)", "Antibiotic", "Atypical infections and acne", "100mg", 13.00f, "Rx" },
    { "Minocycline (Minocin)", "Antibiotic", "Acne and atypical infections", "100mg", 28.00f, "Rx" },
    { "Tetracycline", "Antibiotic", "Broad-spectrum bacterial infections", "250mg", 15.00f, "Rx" },
    { "Gentamicin", "Antibiotic", "Gram-negative serious infections", "80mg", 45.00f, "Rx" },
    { "Tobramycin", "Antibiotic", "Pseudomonas and serious infections", "80mg", 52.00f, "Rx" },
    { "Amikacin", "Antibiotic", "Multi-drug resistant gram-negatives", "500mg", 68.00f, "Rx" },
    { "Meropenem (Merrem)", "Antibiotic", "Broad-spectrum hospital infections", "500mg", 185.00f, "Rx" },
    { "Imipenem-Cilastatin (Primaxin)", "Antibiotic", "Severe polymicrobial infections", "500mg", 175.00f, "Rx" },
    { "Ertapenem (Invanz)", "Antibiotic", "Community-acquired pneumonia IV", "1g", 210.00f, "Rx" },
    { "Vancomycin (Vancocin)", "Antibiotic", "MRSA and resistant gram-positives", "125mg", 55.00f, "Rx" },
    { "Teicoplanin (Targocid)", "Antibiotic", "Gram-positive resistant infections", "200mg", 145.00f, "Rx" },
    { "Clindamycin (Cleocin)", "Antibiotic", "Anaerobic and skin infections", "300mg", 22.00f, "Rx" },
    { "Trimethoprim-Sulfamethoxazole", "Antibiotic", "UTI and PCP treatment", "160/800mg", 12.00f, "Rx" },
    { "Sulfadiazine", "Antibiotic", "Toxoplasmosis and UTI treatment", "500mg", 34.00f, "Rx" },
    { "Metronidazole (Flagyl)", "Antibiotic", "Anaerobic and protozoal infections", "500mg", 16.00f, "Rx" },
    { "Tinidazole (Tindamax)", "Antibiotic", "Bacterial vaginosis and giardia", "500mg", 42.00f, "Rx" },
    { "Linezolid (Zyvox)", "Antibiotic", "MRSA and resistant infections", "600mg", 245.00f, "Rx" },
    { "Tedizolid (Sivextro)", "Antibiotic", "Skin and soft tissue infections", "200mg", 310.00f, "Rx" },
    { "Nitrofurantoin (Macrobid)", "Antibiotic", "Uncomplicated urinary infections", "100mg", 18.00f, "Rx" },
    { "Fosfomycin (Monurol)", "Antibiotic", "Uncomplicated UTI single dose", "3g", 48.00f, "Rx" },
    { "Rifampin (Rifadin)", "Antibiotic", "TB and MRSA combination therapy", "300mg", 56.00f, "Rx" },
    { "Loratadine (Claritin)", "Allergy", "Non-drowsy seasonal allergy relief", "10mg", 12.99f, "OTC" },
    { "Cetirizine (Zyrtec)", "Allergy", "24-hour allergy symptom relief", "10mg", 13.99f, "OTC" },
    { "Fexofenadine (Allegra)", "Allergy", "Non-drowsy perennial allergies", "180mg", 14.99f, "OTC" },
    { "Diphenhydramine (Benadryl)", "Allergy", "Allergy and sleep aid antihistamine", "25mg", 8.49f, "OTC" },
    { "Chlorpheniramine (Chlor-Trimeton)", "Allergy", "First-gen antihistamine for allergies", "4mg", 6.99f, "OTC" },
    { "Levocetirizine (Xyzal)", "Allergy", "Chronic urticaria and rhinitis", "5mg", 19.99f, "OTC" },
    { "Desloratadine (Clarinex)", "Allergy", "Allergic rhinitis treatment", "5mg", 24.00f, "Rx" },
    { "Hydroxyzine (Atarax)", "Allergy", "Allergy and anxiety antihistamine", "25mg", 15.00f, "Rx" },
    { "Fluticasone Nasal (Flonase)", "Allergy", "Nasal allergy corticosteroid spray", "50mcg", 16.99f, "OTC" },
    { "Triamcinolone Nasal (Nasacort)", "Allergy", "Seasonal allergic rhinitis spray", "55mcg", 15.99f, "OTC" },
    { "Mometasone Nasal (Nasonex)", "Allergy", "Nasal congestion and rhinitis", "50mcg", 22.00f, "Rx" },
    { "Budesonide Nasal (Rhinocort)", "Allergy", "Nasal allergy symptom control", "32mcg", 17.99f, "OTC" },
    { "Cromolyn Sodium (NasalCrom)", "Allergy", "Prevents allergic rhinitis symptoms", "5.2mg/spray", 12.99f, "OTC" },
    { "Montelukast (Singulair)", "Allergy", "Allergic rhinitis and asthma", "10mg", 18.00f, "Rx" },
    { "Epinephrine Auto-injector (EpiPen)", "Allergy", "Severe anaphylaxis emergency", "0.3mg", 375.00f, "Rx" },
    { "Omalizumab (Xolair)", "Allergy", "Chronic urticaria biologic therapy", "150mg", 1850.00f, "Rx" },
    { "Dupilumab (Dupixent)", "Allergy", "Atopic dermatitis and asthma", "300mg", 3200.00f, "Rx" },
    { "Azelastine Nasal (Astelin)", "Allergy", "Nasal allergy antihistamine spray", "137mcg", 28.00f, "Rx" },
    { "Olopatadine (Patanol)", "Allergy", "Allergic conjunctivitis eye drops", "0.1%", 34.00f, "Rx" },
    { "Atorvastatin (Lipitor)", "Heart", "LDL cholesterol reduction", "20mg", 18.00f, "Rx" },
    { "Rosuvastatin (Crestor)", "Heart", "High cholesterol and heart disease", "10mg", 22.00f, "Rx" },
    { "Simvastatin (Zocor)", "Heart", "Cholesterol lowering statin", "20mg", 14.00f, "Rx" },
    { "Pravastatin (Pravachol)", "Heart", "Cholesterol reduction statin", "40mg", 16.00f, "Rx" },
    { "Lovastatin (Mevacor)", "Heart", "LDL reduction and prevention", "20mg", 13.00f, "Rx" },
    { "Fluvastatin (Lescol)", "Heart", "Hypercholesterolemia management", "40mg", 24.00f, "Rx" },
    { "Lisinopril (Prinivil)", "Heart", "ACE inhibitor for hypertension", "10mg", 11.00f, "Rx" },
    { "Enalapril (Vasotec)", "Heart", "Heart failure and hypertension", "5mg", 13.00f, "Rx" },
    { "Ramipril (Altace)", "Heart", "Post-MI and hypertension", "5mg", 15.00f, "Rx" },
    { "Captopril (Capoten)", "Heart", "Hypertension and heart failure", "25mg", 12.00f, "Rx" },
    { "Fosinopril (Monopril)", "Heart", "ACE inhibitor for hypertension", "10mg", 14.00f, "Rx" },
    { "Losartan (Cozaar)", "Heart", "ARB for hypertension", "50mg", 16.00f, "Rx" },
    { "Valsartan (Diovan)", "Heart", "Hypertension and heart failure", "80mg", 19.00f, "Rx" },
    { "Olmesartan (Benicar)", "Heart", "ARB antihypertensive therapy", "20mg", 22.00f, "Rx" },
    { "Irbesartan (Avapro)", "Heart", "Hypertension and diabetic nephropathy", "150mg", 21.00f, "Rx" },
    { "Candesartan (Atacand)", "Heart", "ARB for hypertension", "8mg", 24.00f, "Rx" },
    { "Telmisartan (Micardis)", "Heart", "Hypertension and CV risk reduction", "40mg", 26.00f, "Rx" },
    { "Metoprolol Succinate (Toprol XL)", "Heart", "Beta-blocker for heart failure", "25mg", 17.00f, "Rx" },
    { "Metoprolol Tartrate (Lopressor)", "Heart", "Hypertension and angina", "50mg", 14.00f, "Rx" },
    { "Atenolol (Tenormin)", "Heart", "Beta-1 selective hypertension", "50mg", 12.00f, "Rx" },
    { "Carvedilol (Coreg)", "Heart", "Heart failure and hypertension", "6.25mg", 16.00f, "Rx" },
    { "Bisoprolol (Zebeta)", "Heart", "Heart failure beta-blocker", "5mg", 18.00f, "Rx" },
    { "Propranolol (Inderal)", "Heart", "Hypertension and arrhythmia", "40mg", 11.00f, "Rx" },
    { "Labetalol (Trandate)", "Heart", "Alpha/beta blocker hypertension", "200mg", 19.00f, "Rx" },
    { "Amlodipine (Norvasc)", "Heart", "CCB for hypertension and angina", "5mg", 13.00f, "Rx" },
    { "Diltiazem (Cardizem)", "Heart", "Angina and rate control CCB", "120mg", 22.00f, "Rx" },
    { "Verapamil (Calan)", "Heart", "CCB for angina and arrhythmia", "80mg", 18.00f, "Rx" },
    { "Nifedipine (Procardia)", "Heart", "Angina and hypertension CCB", "30mg", 16.00f, "Rx" },
    { "Felodipine (Plendil)", "Heart", "Hypertension calcium channel blocker", "5mg", 20.00f, "Rx" },
    { "Furosemide (Lasix)", "Heart", "Loop diuretic for edema and HF", "40mg", 10.00f, "Rx" },
    { "Bumetanide (Bumex)", "Heart", "Loop diuretic heart failure", "1mg", 14.00f, "Rx" },
    { "Torsemide (Demadex)", "Heart", "Loop diuretic for fluid overload", "10mg", 16.00f, "Rx" },
    { "Hydrochlorothiazide (Microzide)", "Heart", "Thiazide for hypertension", "25mg", 9.00f, "Rx" },
    { "Chlorthalidone (Hygroton)", "Heart", "Long-acting thiazide diuretic", "25mg", 11.00f, "Rx" },
    { "Spironolactone (Aldactone)", "Heart", "Potassium-sparing heart failure", "25mg", 18.00f, "Rx" },
    { "Eplerenone (Inspra)", "Heart", "Aldosterone antagonist post-MI", "25mg", 45.00f, "Rx" },
    { "Amiodarone (Pacerone)", "Heart", "Class III antiarrhythmic", "200mg", 38.00f, "Rx" },
    { "Sotalol (Betapace)", "Heart", "Class III antiarrhythmic AF", "80mg", 32.00f, "Rx" },
    { "Flecainide (Tambocor)", "Heart", "Class IC antiarrhythmic", "100mg", 42.00f, "Rx" },
    { "Propafenone (Rythmol)", "Heart", "Class IC antiarrhythmic AF", "150mg", 48.00f, "Rx" },
    { "Quinidine Sulfate", "Heart", "Class IA antiarrhythmic", "200mg", 35.00f, "Rx" },
    { "Mexiletine (Mexitil)", "Heart", "Class IB ventricular arrhythmia", "150mg", 38.00f, "Rx" },
    { "Adenosine (Adenocard)", "Heart", "SVT conversion IV antiarrhythmic", "6mg/2mL", 85.00f, "Rx" },
    { "Digoxin (Lanoxin)", "Heart", "Heart failure and AF rate control", "0.125mg", 22.00f, "Rx" },
    { "Warfarin (Coumadin)", "Heart", "Anticoagulant for clot prevention", "5mg", 14.00f, "Rx" },
    { "Apixaban (Eliquis)", "Heart", "AF and DVT anticoagulation", "5mg", 178.00f, "Rx" },
    { "Rivaroxaban (Xarelto)", "Heart", "DVT and PE anticoagulation", "20mg", 165.00f, "Rx" },
    { "Dabigatran (Pradaxa)", "Heart", "AF stroke prevention DOAC", "150mg", 158.00f, "Rx" },
    { "Edoxaban (Savaysa)", "Heart", "AF and VTE DOAC treatment", "60mg", 162.00f, "Rx" },
    { "Heparin (Unfractionated)", "Heart", "IV anticoagulation acute clots", "5000 units", 45.00f, "Rx" },
    { "Enoxaparin (Lovenox)", "Heart", "Low molecular weight heparin", "40mg", 78.00f, "Rx" },
    { "Clopidogrel (Plavix)", "Heart", "Antiplatelet post-MI and stroke", "75mg", 22.00f, "Rx" },
    { "Ticagrelor (Brilinta)", "Heart", "Antiplatelet for ACS", "90mg", 145.00f, "Rx" },
    { "Prasugrel (Effient)", "Heart", "Antiplatelet for ACS post-PCI", "10mg", 138.00f, "Rx" },
    { "Aspirin (low-dose)", "Heart", "Antiplatelet cardiovascular prevention", "81mg", 4.99f, "OTC" },
    { "Nitroglycerin (NitroStat)", "Heart", "Angina acute relief sublingual", "0.4mg", 28.00f, "Rx" },
    { "Isosorbide Mononitrate (Imdur)", "Heart", "Nitrate for angina prevention", "30mg", 22.00f, "Rx" },
    { "Isosorbide Dinitrate (Isordil)", "Heart", "Angina and heart failure nitrate", "20mg", 18.00f, "Rx" },
    { "Hydralazine (Apresoline)", "Heart", "Vasodilator for heart failure", "25mg", 14.00f, "Rx" },
    { "Sacubitril-Valsartan (Entresto)", "Heart", "ARNI for heart failure HFrEF", "49/51mg", 385.00f, "Rx" },
    { "Ivabradine (Corlanor)", "Heart", "Heart rate reduction in HF", "5mg", 245.00f, "Rx" },
    { "Dapagliflozin (Farxiga)", "Heart", "SGLT2i heart failure and diabetes", "10mg", 195.00f, "Rx" },
    { "Empagliflozin (Jardiance)", "Heart", "CV death reduction HF/diabetes", "10mg", 198.00f, "Rx" },
    { "Metformin (Glucophage)", "Diabetes", "First-line type 2 diabetes", "500mg", 12.00f, "Rx" },
    { "Glipizide (Glucotrol)", "Diabetes", "Sulfonylurea for type 2 diabetes", "5mg", 14.00f, "Rx" },
    { "Glimepiride (Amaryl)", "Diabetes", "Sulfonylurea blood sugar control", "2mg", 16.00f, "Rx" },
    { "Glyburide (Diabeta)", "Diabetes", "Sulfonylurea glucose control", "5mg", 13.00f, "Rx" },
    { "Sitagliptin (Januvia)", "Diabetes", "DPP-4 inhibitor type 2 diabetes", "100mg", 245.00f, "Rx" },
    { "Saxagliptin (Onglyza)", "Diabetes", "DPP-4 inhibitor for T2DM", "5mg", 238.00f, "Rx" },
    { "Linagliptin (Tradjenta)", "Diabetes", "DPP-4 inhibitor no renal dose adj", "5mg", 242.00f, "Rx" },
    { "Pioglitazone (Actos)", "Diabetes", "Thiazolidinedione insulin sensitizer", "30mg", 28.00f, "Rx" },
    { "Rosiglitazone (Avandia)", "Diabetes", "Insulin sensitizer TZD", "4mg", 35.00f, "Rx" },
    { "Liraglutide (Victoza)", "Diabetes", "GLP-1 agonist glucose control", "1.2mg", 785.00f, "Rx" },
    { "Semaglutide (Ozempic)", "Diabetes", "GLP-1 agonist weekly injection", "0.5mg", 895.00f, "Rx" },
    { "Dulaglutide (Trulicity)", "Diabetes", "Weekly GLP-1 agonist for T2DM", "0.75mg", 845.00f, "Rx" },
    { "Exenatide (Byetta)", "Diabetes", "Twice daily GLP-1 agonist", "5mcg", 345.00f, "Rx" },
    { "Canagliflozin (Invokana)", "Diabetes", "SGLT2 inhibitor glucose reduction", "100mg", 195.00f, "Rx" },
    { "Insulin Glargine (Lantus)", "Diabetes", "Basal long-acting insulin", "100 units/mL", 125.00f, "Rx" },
    { "Insulin Detemir (Levemir)", "Diabetes", "Basal insulin once or twice daily", "100 units/mL", 118.00f, "Rx" },
    { "Insulin Degludec (Tresiba)", "Diabetes", "Ultra-long basal insulin", "100 units/mL", 145.00f, "Rx" },
    { "Insulin Lispro (Humalog)", "Diabetes", "Rapid-acting meal insulin", "100 units/mL", 115.00f, "Rx" },
    { "Insulin Aspart (NovoLog)", "Diabetes", "Rapid-acting prandial insulin", "100 units/mL", 118.00f, "Rx" },
    { "Insulin Glulisine (Apidra)", "Diabetes", "Fast-acting mealtime insulin", "100 units/mL", 112.00f, "Rx" },
    { "Regular Insulin (Humulin R)", "Diabetes", "Short-acting human insulin", "100 units/mL", 98.00f, "Rx" },
    { "NPH Insulin (Humulin N)", "Diabetes", "Intermediate-acting insulin", "100 units/mL", 92.00f, "Rx" },
    { "Acarbose (Precose)", "Diabetes", "Alpha-glucosidase inhibitor T2DM", "50mg", 28.00f, "Rx" },
    { "Repaglinide (Prandin)", "Diabetes", "Meglitinide short-acting secretagogue", "1mg", 45.00f, "Rx" },
    { "Omeprazole (Prilosec)", "Digestive", "PPI for GERD and ulcers", "20mg", 14.99f, "OTC" },
    { "Esomeprazole (Nexium)", "Digestive", "PPI for erosive esophagitis", "40mg", 19.99f, "OTC" },
    { "Pantoprazole (Protonix)", "Digestive", "PPI for GERD and H. pylori", "40mg", 16.00f, "Rx" },
    { "Lansoprazole (Prevacid)", "Digestive", "PPI for ulcers and GERD", "30mg", 15.99f, "OTC" },
    { "Rabeprazole (Aciphex)", "Digestive", "PPI for GERD treatment", "20mg", 22.00f, "Rx" },
    { "Famotidine (Pepcid)", "Digestive", "H2 blocker for heartburn", "20mg", 10.99f, "OTC" },
    { "Ranitidine (Zantac)", "Digestive", "H2 blocker acid reduction", "150mg", 11.99f, "OTC" },
    { "Cimetidine (Tagamet)", "Digestive", "H2 blocker for peptic ulcers", "400mg", 9.99f, "OTC" },
    { "Sucralfate (Carafate)", "Digestive", "Mucosal protection for ulcers", "1g", 22.00f, "Rx" },
    { "Bismuth Subsalicylate (Pepto)", "Digestive", "Indigestion and traveler's diarrhea", "262mg", 8.99f, "OTC" },
    { "Ondansetron (Zofran)", "Digestive", "Nausea and vomiting antiemetic", "4mg", 24.00f, "Rx" },
    { "Promethazine (Phenergan)", "Digestive", "Nausea antihistamine antiemetic", "25mg", 18.00f, "Rx" },
    { "Metoclopramide (Reglan)", "Digestive", "Gastroparesis and nausea", "10mg", 16.00f, "Rx" },
    { "Prochlorperazine (Compazine)", "Digestive", "Severe nausea and vomiting", "10mg", 21.00f, "Rx" },
    { "Domperidone (Motilium)", "Digestive", "Gastroparesis motility agent", "10mg", 28.00f, "Rx" },
    { "Loperamide (Imodium)", "Digestive", "Antidiarrheal agent", "2mg", 7.99f, "OTC" },
    { "Diphenoxylate-Atropine (Lomotil)", "Digestive", "Opioid-type antidiarrheal", "2.5mg", 22.00f, "Rx" },
    { "Psyllium Husk (Metamucil)", "Digestive", "Fiber laxative for constipation", "3.4g", 11.99f, "OTC" },
    { "Polyethylene Glycol (MiraLax)", "Digestive", "Osmotic laxative constipation", "17g", 13.99f, "OTC" },
    { "Docusate Sodium (Colace)", "Digestive", "Stool softener for constipation", "100mg", 9.99f, "OTC" },
    { "Bisacodyl (Dulcolax)", "Digestive", "Stimulant laxative for constipation", "5mg", 10.99f, "OTC" },
    { "Lubiprostone (Amitiza)", "Digestive", "CIC and IBS-C secretagogue", "24mcg", 285.00f, "Rx" },
    { "Linaclotide (Linzess)", "Digestive", "IBS-C and CIC secretagogue", "145mcg", 295.00f, "Rx" },
    { "Mesalamine (Asacol HD)", "Digestive", "Ulcerative colitis treatment", "800mg", 145.00f, "Rx" },
    { "Sulfasalazine (Azulfidine)", "Digestive", "IBD and rheumatoid arthritis", "500mg", 28.00f, "Rx" },
    { "Budesonide (Entocort EC)", "Digestive", "Crohn's disease oral steroid", "3mg", 85.00f, "Rx" },
    { "Infliximab (Remicade)", "Digestive", "TNF inhibitor for IBD", "100mg", 1450.00f, "Rx" },
    { "Adalimumab (Humira)", "Digestive", "TNF inhibitor Crohn's/UC", "40mg", 1850.00f, "Rx" },
    { "Vedolizumab (Entyvio)", "Digestive", "Gut-selective biologic for IBD", "300mg", 2800.00f, "Rx" },
    { "Ursodiol (Actigall)", "Digestive", "Bile acid for gallstones and PBC", "300mg", 45.00f, "Rx" },
    { "Cholestyramine (Questran)", "Digestive", "Bile acid sequestrant diarrhea", "4g", 32.00f, "Rx" },
    { "Rifaximin (Xifaxan)", "Digestive", "IBS-D and traveler's diarrhea", "550mg", 185.00f, "Rx" },
    { "Sertraline (Zoloft)", "Mental Health", "SSRI for depression and anxiety", "50mg", 18.00f, "Rx" },
    { "Fluoxetine (Prozac)", "Mental Health", "SSRI for depression and OCD", "20mg", 16.00f, "Rx" },
    { "Escitalopram (Lexapro)", "Mental Health", "SSRI for depression and GAD", "10mg", 22.00f, "Rx" },
    { "Citalopram (Celexa)", "Mental Health", "SSRI for depression", "20mg", 15.00f, "Rx" },
    { "Paroxetine (Paxil)", "Mental Health", "SSRI for depression and panic", "20mg", 19.00f, "Rx" },
    { "Fluvoxamine (Luvox)", "Mental Health", "SSRI for OCD treatment", "100mg", 24.00f, "Rx" },
    { "Venlafaxine (Effexor XR)", "Mental Health", "SNRI for depression and anxiety", "75mg", 28.00f, "Rx" },
    { "Duloxetine (Cymbalta)", "Mental Health", "SNRI for depression and pain", "60mg", 32.00f, "Rx" },
    { "Desvenlafaxine (Pristiq)", "Mental Health", "SNRI for major depression", "50mg", 38.00f, "Rx" },
    { "Levomilnacipran (Fetzima)", "Mental Health", "SNRI for major depression", "40mg", 45.00f, "Rx" },
    { "Milnacipran (Savella)", "Mental Health", "SNRI for fibromyalgia", "50mg", 42.00f, "Rx" },
    { "Amitriptyline (Elavil)", "Mental Health", "TCA depression and neuropathy", "25mg", 14.00f, "Rx" },
    { "Nortriptyline (Pamelor)", "Mental Health", "TCA for depression", "25mg", 16.00f, "Rx" },
    { "Imipramine (Tofranil)", "Mental Health", "TCA for depression and enuresis", "25mg", 18.00f, "Rx" },
    { "Doxepin (Sinequan)", "Mental Health", "TCA for depression and insomnia", "25mg", 22.00f, "Rx" },
    { "Clomipramine (Anafranil)", "Mental Health", "TCA for OCD treatment", "50mg", 28.00f, "Rx" },
    { "Phenelzine (Nardil)", "Mental Health", "MAOI for atypical depression", "15mg", 35.00f, "Rx" },
    { "Tranylcypromine (Parnate)", "Mental Health", "MAOI for treatment-resistant MDD", "10mg", 38.00f, "Rx" },
    { "Selegiline Patch (Emsam)", "Mental Health", "MAOI transdermal for depression", "6mg/24hr", 185.00f, "Rx" },
    { "Bupropion (Wellbutrin XL)", "Mental Health", "Atypical antidepressant and smoking", "150mg", 38.00f, "Rx" },
    { "Mirtazapine (Remeron)", "Mental Health", "NaSSA for depression and insomnia", "15mg", 22.00f, "Rx" },
    { "Trazodone (Desyrel)", "Mental Health", "SARI for depression and sleep", "100mg", 18.00f, "Rx" },
    { "Vilazodone (Viibryd)", "Mental Health", "SPARI for major depression", "20mg", 58.00f, "Rx" },
    { "Vortioxetine (Trintellix)", "Mental Health", "Multimodal antidepressant", "10mg", 65.00f, "Rx" },
    { "Haloperidol (Haldol)", "Mental Health", "Typical antipsychotic for psychosis", "5mg", 18.00f, "Rx" },
    { "Chlorpromazine (Thorazine)", "Mental Health", "First-gen antipsychotic", "100mg", 22.00f, "Rx" },
    { "Perphenazine (Trilafon)", "Mental Health", "Typical antipsychotic", "8mg", 28.00f, "Rx" },
    { "Fluphenazine (Prolixin)", "Mental Health", "Typical antipsychotic depot/oral", "5mg", 24.00f, "Rx" },
    { "Thioridazine (Mellaril)", "Mental Health", "Typical antipsychotic", "100mg", 26.00f, "Rx" },
    { "Risperidone (Risperdal)", "Mental Health", "Atypical antipsychotic schizophrenia", "2mg", 28.00f, "Rx" },
    { "Olanzapine (Zyprexa)", "Mental Health", "Atypical antipsychotic and bipolar", "10mg", 45.00f, "Rx" },
    { "Quetiapine (Seroquel)", "Mental Health", "Atypical antipsychotic and bipolar", "100mg", 38.00f, "Rx" },
    { "Aripiprazole (Abilify)", "Mental Health", "Atypical antipsychotic adjunct", "10mg", 55.00f, "Rx" },
    { "Ziprasidone (Geodon)", "Mental Health", "Atypical antipsychotic", "40mg", 42.00f, "Rx" },
    { "Clozapine (Clozaril)", "Mental Health", "Treatment-resistant schizophrenia", "100mg", 145.00f, "Rx" },
    { "Asenapine (Saphris)", "Mental Health", "Atypical antipsychotic sublingual", "5mg", 68.00f, "Rx" },
    { "Lurasidone (Latuda)", "Mental Health", "Bipolar depression and schizophrenia", "40mg", 95.00f, "Rx" },
    { "Brexpiprazole (Rexulti)", "Mental Health", "Adjunct antidepressant treatment", "2mg", 145.00f, "Rx" },
    { "Cariprazine (Vraylar)", "Mental Health", "Bipolar and schizophrenia", "1.5mg", 155.00f, "Rx" },
    { "Lithium Carbonate (Eskalith)", "Mental Health", "Mood stabilizer for bipolar", "300mg", 22.00f, "Rx" },
    { "Valproic Acid (Depakote)", "Mental Health", "Mood stabilizer and anticonvulsant", "500mg", 28.00f, "Rx" },
    { "Lamotrigine (Lamictal)", "Mental Health", "Bipolar and epilepsy mood stabilizer", "100mg", 24.00f, "Rx" },
    { "Carbamazepine (Tegretol)", "Mental Health", "Mood stabilizer and anticonvulsant", "200mg", 22.00f, "Rx" },
    { "Oxcarbazepine (Trileptal)", "Mental Health", "Mood stabilizer alternative", "300mg", 28.00f, "Rx" },
    { "Methylphenidate (Ritalin)", "Mental Health", "ADHD stimulant treatment", "10mg", 38.00f, "Rx" },
    { "Amphetamine/Dextroamphetamine (Adderall)", "Mental Health", "ADHD and narcolepsy stimulant", "10mg", 42.00f, "Rx" },
    { "Lisdexamfetamine (Vyvanse)", "Mental Health", "ADHD prodrug stimulant", "30mg", 115.00f, "Rx" },
    { "Atomoxetine (Strattera)", "Mental Health", "Non-stimulant ADHD treatment", "40mg", 85.00f, "Rx" },
    { "Guanfacine (Intuniv)", "Mental Health", "Non-stimulant ADHD alpha-2 agonist", "1mg", 65.00f, "Rx" },
    { "Clonidine (Kapvay)", "Mental Health", "ADHD non-stimulant alpha agonist", "0.1mg", 35.00f, "Rx" },
    { "Lorazepam (Ativan)", "Mental Health", "Benzodiazepine anxiolytic", "1mg", 18.00f, "Rx" },
    { "Diazepam (Valium)", "Mental Health", "Benzo for anxiety and seizures", "5mg", 16.00f, "Rx" },
    { "Alprazolam (Xanax)", "Mental Health", "Panic disorder and anxiety benzo", "0.5mg", 22.00f, "Rx" },
    { "Clonazepam (Klonopin)", "Mental Health", "Panic disorder and seizures benzo", "1mg", 19.00f, "Rx" },
    { "Buspirone (Buspar)", "Mental Health", "Non-benzo anxiolytic for GAD", "10mg", 16.00f, "Rx" },
    { "Naltrexone (Vivitrol)", "Mental Health", "Alcohol and opioid use disorder", "50mg", 145.00f, "Rx" },
    { "Buprenorphine-Naloxone (Suboxone)", "Mental Health", "Opioid use disorder treatment", "8/2mg", 185.00f, "Rx" },
    { "Acamprosate (Campral)", "Mental Health", "Alcohol dependence maintenance", "666mg", 88.00f, "Rx" },
    { "Disulfiram (Antabuse)", "Mental Health", "Alcohol deterrent therapy", "250mg", 42.00f, "Rx" },
    { "Vitamin A (Retinol)", "Vitamin", "Vision and immune function support", "10000 IU", 8.99f, "OTC" },
    { "Vitamin B1 (Thiamine)", "Vitamin", "Nerve function and metabolism", "100mg", 7.99f, "OTC" },
    { "Vitamin B2 (Riboflavin)", "Vitamin", "Energy metabolism support", "100mg", 7.99f, "OTC" },
    { "Vitamin B3 (Niacin)", "Vitamin", "Cholesterol and energy metabolism", "500mg", 9.99f, "OTC" },
    { "Vitamin B5 (Pantothenic Acid)", "Vitamin", "Coenzyme A synthesis support", "500mg", 8.49f, "OTC" },
    { "Vitamin B6 (Pyridoxine)", "Vitamin", "Protein metabolism and immunity", "100mg", 7.99f, "OTC" },
    { "Vitamin B7 (Biotin)", "Vitamin", "Hair and nail growth support", "5000mcg", 9.99f, "OTC" },
    { "Vitamin B9 (Folic Acid)", "Vitamin", "Neural tube defect prevention", "400mcg", 6.99f, "OTC" },
    { "Vitamin B12 (Cyanocobalamin)", "Vitamin", "Nerve health and red blood cells", "1000mcg", 8.99f, "OTC" },
    { "Vitamin C (Ascorbic Acid)", "Vitamin", "Antioxidant and immune support", "1000mg", 9.49f, "OTC" },
    { "Vitamin D3 (Cholecalciferol)", "Vitamin", "Bone health and immune function", "2000 IU", 8.99f, "OTC" },
    { "Vitamin D2 (Ergocalciferol)", "Vitamin", "Vitamin D deficiency treatment", "50000 IU", 14.00f, "Rx" },
    { "Vitamin E (Tocopherol)", "Vitamin", "Antioxidant cell protection", "400 IU", 8.99f, "OTC" },
    { "Vitamin K1 (Phytonadione)", "Vitamin", "Blood clotting and bone health", "100mcg", 9.99f, "OTC" },
    { "Vitamin K2 (Menaquinone)", "Vitamin", "Bone and cardiovascular health", "100mcg", 12.99f, "OTC" },
    { "Calcium Carbonate (Tums)", "Vitamin", "Bone health and antacid", "500mg", 8.99f, "OTC" },
    { "Calcium Citrate (Citracal)", "Vitamin", "Bone density and calcium source", "630mg", 12.99f, "OTC" },
    { "Magnesium Glycinate", "Vitamin", "Muscle relaxation and sleep", "400mg", 14.99f, "OTC" },
    { "Magnesium Oxide", "Vitamin", "Constipation and magnesium supplement", "400mg", 9.99f, "OTC" },
    { "Zinc Gluconate", "Vitamin", "Immune support and wound healing", "50mg", 8.99f, "OTC" },
    { "Iron (Ferrous Sulfate)", "Vitamin", "Iron deficiency anemia treatment", "325mg", 7.99f, "OTC" },
    { "Selenium", "Vitamin", "Antioxidant thyroid function support", "200mcg", 8.49f, "OTC" },
    { "Chromium Picolinate", "Vitamin", "Blood sugar and metabolism support", "200mcg", 9.99f, "OTC" },
    { "Iodine (Potassium Iodide)", "Vitamin", "Thyroid function support", "150mcg", 7.99f, "OTC" },
    { "Potassium Chloride", "Vitamin", "Potassium supplementation", "99mg", 8.49f, "OTC" },
    { "Omega-3 Fish Oil", "Vitamin", "Heart and brain health fatty acids", "1000mg", 15.99f, "OTC" },
    { "Flaxseed Oil (ALA)", "Vitamin", "Plant-based omega-3 fatty acids", "1000mg", 12.99f, "OTC" },
    { "Probiotics (Lactobacillus)", "Vitamin", "Gut microbiome health support", "10 billion CFU", 18.99f, "OTC" },
    { "Coenzyme Q10 (CoQ10)", "Vitamin", "Cellular energy and antioxidant", "200mg", 19.99f, "OTC" },
    { "L-Glutamine", "Vitamin", "Gut health and muscle recovery", "5g", 14.99f, "OTC" },
    { "L-Arginine", "Vitamin", "Nitric oxide and circulation support", "500mg", 12.99f, "OTC" },
    { "L-Carnitine", "Vitamin", "Fat metabolism and energy", "500mg", 14.99f, "OTC" },
    { "Collagen Peptides", "Vitamin", "Joint and skin health support", "10g", 22.99f, "OTC" },
    { "Glucosamine Sulfate", "Vitamin", "Joint cartilage health support", "1500mg", 16.99f, "OTC" },
    { "Chondroitin Sulfate", "Vitamin", "Joint pain and osteoarthritis", "1200mg", 18.99f, "OTC" },
    { "Echinacea", "Vitamin", "Herbal immune function support", "400mg", 11.99f, "OTC" },
    { "St. John's Wort", "Vitamin", "Herbal mild depression support", "300mg", 12.99f, "OTC" },
    { "Valerian Root", "Vitamin", "Herbal sleep and anxiety support", "500mg", 11.99f, "OTC" },
    { "Turmeric (Curcumin)", "Vitamin", "Herbal anti-inflammatory support", "500mg", 13.99f, "OTC" },
    { "Ashwagandha (KSM-66)", "Vitamin", "Adaptogen stress and anxiety relief", "600mg", 15.99f, "OTC" },
    { "Albuterol (ProAir HFA)", "Respiratory", "SABA for acute bronchospasm", "90mcg/puff", 45.00f, "Rx" },
    { "Levalbuterol (Xopenex)", "Respiratory", "SABA with fewer side effects", "45mcg/puff", 55.00f, "Rx" },
    { "Pirbuterol (Maxair)", "Respiratory", "SABA bronchodilator inhaler", "200mcg/puff", 48.00f, "Rx" },
    { "Salmeterol (Serevent)", "Respiratory", "LABA for asthma maintenance", "50mcg/puff", 78.00f, "Rx" },
    { "Formoterol (Foradil)", "Respiratory", "LABA bronchodilator for asthma", "12mcg/puff", 82.00f, "Rx" },
    { "Indacaterol (Arcapta)", "Respiratory", "Once-daily LABA for COPD", "75mcg", 185.00f, "Rx" },
    { "Ipratropium (Atrovent)", "Respiratory", "SAMA anticholinergic bronchodilator", "17mcg/puff", 62.00f, "Rx" },
    { "Tiotropium (Spiriva)", "Respiratory", "LAMA once-daily for COPD", "18mcg", 145.00f, "Rx" },
    { "Umeclidinium (Incruse)", "Respiratory", "Once-daily LAMA for COPD", "62.5mcg", 155.00f, "Rx" },
    { "Glycopyrrolate (Seebri)", "Respiratory", "LAMA for COPD maintenance", "15.6mcg", 148.00f, "Rx" },
    { "Fluticasone Propionate (Flovent)", "Respiratory", "ICS for asthma maintenance", "110mcg", 85.00f, "Rx" },
    { "Budesonide (Pulmicort)", "Respiratory", "ICS for asthma maintenance", "180mcg", 88.00f, "Rx" },
    { "Beclomethasone (Qvar)", "Respiratory", "ICS asthma anti-inflammatory", "80mcg", 82.00f, "Rx" },
    { "Mometasone (Asmanex)", "Respiratory", "ICS once-daily asthma control", "220mcg", 95.00f, "Rx" },
    { "Ciclesonide (Alvesco)", "Respiratory", "ICS pro-drug for asthma", "80mcg", 92.00f, "Rx" },
    { "Fluticasone-Salmeterol (Advair)", "Respiratory", "ICS/LABA combo for asthma/COPD", "250/50mcg", 145.00f, "Rx" },
    { "Budesonide-Formoterol (Symbicort)", "Respiratory", "ICS/LABA combo inhaler", "160/4.5mcg", 138.00f, "Rx" },
    { "Mometasone-Formoterol (Dulera)", "Respiratory", "ICS/LABA combo for asthma", "100/5mcg", 148.00f, "Rx" },
    { "Fluticasone-Vilanterol (Breo)", "Respiratory", "Once-daily ICS/LABA for COPD", "100/25mcg", 185.00f, "Rx" },
    { "Umeclidinium-Vilanterol (Anoro)", "Respiratory", "LAMA/LABA combo for COPD", "62.5/25mcg", 195.00f, "Rx" },
    { "Roflumilast (Daliresp)", "Respiratory", "PDE4 inhibitor for COPD", "500mcg", 245.00f, "Rx" },
    { "Theophylline (Uniphyl)", "Respiratory", "Bronchodilator for asthma/COPD", "400mg", 28.00f, "Rx" },
    { "Guaifenesin (Mucinex)", "Respiratory", "Expectorant for chest congestion", "600mg", 11.99f, "OTC" },
    { "N-Acetylcysteine (Mucomyst)", "Respiratory", "Mucolytic for COPD secretions", "600mg", 18.00f, "Rx" },
    { "Dextromethorphan (Delsym)", "Respiratory", "Antitussive cough suppressant", "30mg", 10.99f, "OTC" },
    { "Benzonatate (Tessalon)", "Respiratory", "Non-narcotic cough suppressant", "200mg", 18.00f, "Rx" },
    { "Mepolizumab (Nucala)", "Respiratory", "Anti-IL-5 biologic for asthma", "100mg", 2850.00f, "Rx" },
    { "Benralizumab (Fasenra)", "Respiratory", "Anti-IL-5Ra biologic asthma", "30mg", 3100.00f, "Rx" },
    { "Reslizumab (Cinqair)", "Respiratory", "Anti-IL-5 IV biologic for asthma", "100mg/10mL", 2950.00f, "Rx" },
    { "Tezepelumab (Tezspire)", "Respiratory", "Anti-TSLP biologic for asthma", "210mg", 3250.00f, "Rx" },
    { "Hydrocortisone Cream (Cortaid)", "Skin", "Mild topical corticosteroid itch", "1%", 7.99f, "OTC" },
    { "Triamcinolone Cream (Kenalog)", "Skin", "Mid-potency topical steroid", "0.1%", 16.00f, "Rx" },
    { "Betamethasone (Diprolene)", "Skin", "High-potency topical steroid", "0.05%", 22.00f, "Rx" },
    { "Clobetasol (Temovate)", "Skin", "Super-potent topical corticosteroid", "0.05%", 28.00f, "Rx" },
    { "Mometasone (Elocon)", "Skin", "Mid-potency topical steroid cream", "0.1%", 24.00f, "Rx" },
    { "Fluocinonide (Lidex)", "Skin", "High-potency topical steroid", "0.05%", 25.00f, "Rx" },
    { "Desonide (DesOwen)", "Skin", "Low-potency topical steroid eczema", "0.05%", 19.00f, "Rx" },
    { "Tretinoin (Retin-A)", "Skin", "Retinoid for acne and anti-aging", "0.025%", 38.00f, "Rx" },
    { "Adapalene (Differin)", "Skin", "Topical retinoid for acne", "0.1%", 17.99f, "OTC" },
    { "Tazarotene (Tazorac)", "Skin", "Retinoid for acne and psoriasis", "0.05%", 95.00f, "Rx" },
    { "Benzoyl Peroxide (PanOxyl)", "Skin", "Acne antibacterial treatment", "5%", 9.99f, "OTC" },
    { "Salicylic Acid (Stridex)", "Skin", "Acne keratolytic treatment", "2%", 8.99f, "OTC" },
    { "Clindamycin Topical (Cleocin T)", "Skin", "Topical antibiotic for acne", "1%", 22.00f, "Rx" },
    { "Erythromycin Topical", "Skin", "Topical antibiotic acne treatment", "2%", 20.00f, "Rx" },
    { "Doxycycline (Oracea)", "Skin", "Low-dose for rosacea treatment", "40mg", 145.00f, "Rx" },
    { "Isotretinoin (Accutane)", "Skin", "Severe cystic acne treatment", "20mg", 185.00f, "Rx" },
    { "Metronidazole Gel (MetroGel)", "Skin", "Rosacea topical treatment", "0.75%", 85.00f, "Rx" },
    { "Azelaic Acid (Finacea)", "Skin", "Rosacea and acne treatment", "15%", 78.00f, "Rx" },
    { "Mupirocin (Bactroban)", "Skin", "Topical antibiotic for impetigo", "2%", 28.00f, "Rx" },
    { "Clotrimazole Cream (Lotrimin)", "Skin", "Topical antifungal for ringworm", "1%", 9.99f, "OTC" },
    { "Terbinafine Cream (Lamisil AT)", "Skin", "Topical antifungal athlete's foot", "1%", 12.99f, "OTC" },
    { "Ketoconazole Cream (Nizoral)", "Skin", "Antifungal for tinea infections", "2%", 18.00f, "Rx" },
    { "Calcipotriene (Dovonex)", "Skin", "Vitamin D analog for psoriasis", "0.005%", 125.00f, "Rx" },
    { "Coal Tar (Neutrogena T/Gel)", "Skin", "Psoriasis and dandruff shampoo", "1%", 11.99f, "OTC" },
    { "Tacrolimus (Protopic)", "Skin", "Calcineurin inhibitor for eczema", "0.1%", 185.00f, "Rx" },
    { "Pimecrolimus (Elidel)", "Skin", "Calcineurin inhibitor atopic derm", "1%", 175.00f, "Rx" },
    { "Secukinumab (Cosentyx)", "Skin", "IL-17A biologic for psoriasis", "150mg", 3450.00f, "Rx" },
    { "Ustekinumab (Stelara)", "Skin", "IL-12/23 biologic for psoriasis", "45mg", 3850.00f, "Rx" },
    { "Ixekizumab (Taltz)", "Skin", "IL-17A biologic for psoriasis", "80mg", 3650.00f, "Rx" },
    { "Phenytoin (Dilantin)", "Neurological", "Seizure control anticonvulsant", "100mg", 22.00f, "Rx" },
    { "Valproate (Depakote ER)", "Neurological", "Epilepsy and migraine prevention", "500mg", 28.00f, "Rx" },
    { "Levetiracetam (Keppra)", "Neurological", "Broad-spectrum anticonvulsant", "500mg", 22.00f, "Rx" },
    { "Topiramate (Topamax)", "Neurological", "Epilepsy and migraine prevention", "100mg", 26.00f, "Rx" },
    { "Zonisamide (Zonegran)", "Neurological", "Adjunct anticonvulsant therapy", "100mg", 32.00f, "Rx" },
    { "Lacosamide (Vimpat)", "Neurological", "Focal seizure anticonvulsant", "100mg", 145.00f, "Rx" },
    { "Perampanel (Fycompa)", "Neurological", "AMPA receptor antagonist seizures", "4mg", 185.00f, "Rx" },
    { "Clobazam (Onfi)", "Neurological", "Lennox-Gastaut seizure adjunct", "10mg", 95.00f, "Rx" },
    { "Vigabatrin (Sabril)", "Neurological", "Infantile spasms anticonvulsant", "500mg", 245.00f, "Rx" },
    { "Rufinamide (Banzel)", "Neurological", "Lennox-Gastaut syndrome adjunct", "400mg", 195.00f, "Rx" },
    { "Brivaracetam (Briviact)", "Neurological", "Focal-onset seizure adjunct", "50mg", 165.00f, "Rx" },
    { "Cannabidiol (Epidiolex)", "Neurological", "Dravet and LGS seizure control", "100mg/mL", 285.00f, "Rx" },
    { "Levodopa-Carbidopa (Sinemet)", "Neurological", "Parkinson's disease treatment", "25/100mg", 32.00f, "Rx" },
    { "Pramipexole (Mirapex)", "Neurological", "Parkinson's dopamine agonist", "0.5mg", 42.00f, "Rx" },
    { "Ropinirole (Requip)", "Neurological", "Parkinson's and RLS dopamine agonist", "1mg", 38.00f, "Rx" },
    { "Rasagiline (Azilect)", "Neurological", "MAO-B inhibitor for Parkinson's", "1mg", 85.00f, "Rx" },
    { "Selegiline (Eldepryl)", "Neurological", "MAO-B inhibitor Parkinson's adjunct", "5mg", 45.00f, "Rx" },
    { "Entacapone (Comtan)", "Neurological", "COMT inhibitor Parkinson's adjunct", "200mg", 65.00f, "Rx" },
    { "Amantadine (Symmetrel)", "Neurological", "Parkinson's dyskinesia treatment", "100mg", 28.00f, "Rx" },
    { "Sumatriptan (Imitrex)", "Neurological", "Triptan for acute migraine", "50mg", 35.00f, "Rx" },
    { "Rizatriptan (Maxalt)", "Neurological", "Triptan for migraine acute relief", "10mg", 38.00f, "Rx" },
    { "Eletriptan (Relpax)", "Neurological", "Triptan for migraine attack", "40mg", 42.00f, "Rx" },
    { "Almotriptan (Axert)", "Neurological", "Triptan for acute migraine", "12.5mg", 40.00f, "Rx" },
    { "Ergotamine-Caffeine (Cafergot)", "Neurological", "Ergot for acute migraine", "1/100mg", 28.00f, "Rx" },
    { "Dihydroergotamine (Migranal)", "Neurological", "Ergot nasal spray for migraine", "0.5mg/spray", 65.00f, "Rx" },
    { "Amitriptyline", "Neurological", "TCA migraine prevention", "10mg", 14.00f, "Rx" },
    { "Erenumab (Aimovig)", "Neurological", "CGRP mAb migraine prevention", "70mg", 585.00f, "Rx" },
    { "Fremanezumab (Ajovy)", "Neurological", "CGRP mAb monthly migraine prevent", "225mg", 578.00f, "Rx" },
    { "Galcanezumab (Emgality)", "Neurological", "CGRP mAb migraine prevention", "120mg", 582.00f, "Rx" },
    { "Donepezil (Aricept)", "Neurological", "AChE inhibitor for Alzheimer's", "10mg", 28.00f, "Rx" },
    { "Rivastigmine (Exelon)", "Neurological", "AChE inhibitor dementia patch/oral", "4.6mg/24hr", 45.00f, "Rx" },
    { "Galantamine (Razadyne)", "Neurological", "AChE inhibitor for Alzheimer's", "8mg", 35.00f, "Rx" },
    { "Memantine (Namenda)", "Neurological", "NMDA antagonist for Alzheimer's", "10mg", 32.00f, "Rx" },
    { "Interferon beta-1a (Avonex)", "Neurological", "MS disease-modifying therapy", "30mcg", 1850.00f, "Rx" },
    { "Glatiramer (Copaxone)", "Neurological", "MS immunomodulator injection", "20mg", 1650.00f, "Rx" },
    { "Natalizumab (Tysabri)", "Neurological", "Anti-VLA-4 mAb for MS", "300mg", 2850.00f, "Rx" },
    { "Ocrelizumab (Ocrevus)", "Neurological", "Anti-CD20 biologic for MS", "300mg", 3250.00f, "Rx" },
    { "Levothyroxine (Synthroid)", "Hormonal", "Hypothyroidism thyroid hormone", "50mcg", 14.00f, "Rx" },
    { "Liothyronine (Cytomel)", "Hormonal", "Active T3 thyroid hormone", "25mcg", 22.00f, "Rx" },
    { "Desiccated Thyroid (Armour)", "Hormonal", "Natural thyroid replacement", "60mg", 28.00f, "Rx" },
    { "Methimazole (Tapazole)", "Hormonal", "Hyperthyroidism antithyroid drug", "10mg", 18.00f, "Rx" },
    { "Propylthiouracil (PTU)", "Hormonal", "Graves disease antithyroid", "50mg", 16.00f, "Rx" },
    { "Prednisone (Deltasone)", "Hormonal", "Oral corticosteroid anti-inflammatory", "20mg", 12.00f, "Rx" },
    { "Methylprednisolone (Medrol)", "Hormonal", "Corticosteroid for inflammation", "4mg", 18.00f, "Rx" },
    { "Dexamethasone (Decadron)", "Hormonal", "Potent corticosteroid inflammation", "4mg", 16.00f, "Rx" },
    { "Fludrocortisone (Florinef)", "Hormonal", "Mineralocorticoid for Addison's", "0.1mg", 45.00f, "Rx" },
    { "Hydrocortisone (Cortef)", "Hormonal", "Adrenal insufficiency replacement", "20mg", 22.00f, "Rx" },
    { "Testosterone Cypionate", "Hormonal", "Androgen replacement therapy IM", "200mg/mL", 68.00f, "Rx" },
    { "Testosterone Gel (AndroGel)", "Hormonal", "Topical androgen replacement", "1% gel", 185.00f, "Rx" },
    { "Estradiol (Estrace)", "Hormonal", "Estrogen replacement therapy", "1mg", 22.00f, "Rx" },
    { "Conjugated Estrogens (Premarin)", "Hormonal", "Menopause estrogen replacement", "0.625mg", 28.00f, "Rx" },
    { "Progesterone (Prometrium)", "Hormonal", "Progestogen hormonal therapy", "200mg", 45.00f, "Rx" },
    { "Medroxyprogesterone (Provera)", "Hormonal", "Progestin for hormonal therapy", "10mg", 24.00f, "Rx" },
    { "Growth Hormone (Norditropin)", "Hormonal", "GH deficiency replacement therapy", "5mg/1.5mL", 1850.00f, "Rx" },
    { "Octreotide (Sandostatin)", "Hormonal", "Somatostatin analog acromegaly", "100mcg", 145.00f, "Rx" },
    { "Desmopressin (DDAVP)", "Hormonal", "ADH analog for diabetes insipidus", "0.1mg", 65.00f, "Rx" },
    { "Bromocriptine (Parlodel)", "Hormonal", "Dopamine agonist hyperprolactinemia", "2.5mg", 45.00f, "Rx" },
    { "Cabergoline (Dostinex)", "Hormonal", "Dopamine agonist prolactinoma", "0.5mg", 85.00f, "Rx" },
    { "Acyclovir (Zovirax)", "Antiviral", "Herpes simplex and zoster treatment", "400mg", 18.00f, "Rx" },
    { "Valacyclovir (Valtrex)", "Antiviral", "Herpes prodrug oral treatment", "500mg", 28.00f, "Rx" },
    { "Famciclovir (Famvir)", "Antiviral", "Herpes zoster and HSV treatment", "250mg", 35.00f, "Rx" },
    { "Oseltamivir (Tamiflu)", "Antiviral", "Influenza treatment and prevention", "75mg", 45.00f, "Rx" },
    { "Zanamivir (Relenza)", "Antiviral", "Influenza inhaled antiviral", "5mg/blister", 42.00f, "Rx" },
    { "Baloxavir (Xofluza)", "Antiviral", "Cap-dependent endonuclease inhibitor", "40mg", 85.00f, "Rx" },
    { "Tenofovir-Emtricitabine (Truvada)", "Antiviral", "HIV PrEP and NRTI combination", "300/200mg", 1850.00f, "Rx" },
    { "Bictegravir-TAF-FTC (Biktarvy)", "Antiviral", "HIV integrase inhibitor regimen", "50/25/200mg", 3850.00f, "Rx" },
    { "Dolutegravir (Tivicay)", "Antiviral", "HIV integrase strand transfer inhib", "50mg", 1650.00f, "Rx" },
    { "Efavirenz (Sustiva)", "Antiviral", "HIV NNRTI antiretroviral", "600mg", 485.00f, "Rx" },
    { "Ritonavir (Norvir)", "Antiviral", "HIV protease inhibitor booster", "100mg", 145.00f, "Rx" },
    { "Lopinavir-Ritonavir (Kaletra)", "Antiviral", "HIV protease inhibitor combination", "200/50mg", 385.00f, "Rx" },
    { "Sofosbuvir-Velpatasvir (Epclusa)", "Antiviral", "Pan-genotypic HCV treatment", "400/100mg", 18500.00f, "Rx" },
    { "Glecaprevir-Pibrentasvir (Mavyret)", "Antiviral", "Pan-genotypic HCV 8-week course", "100/40mg", 16500.00f, "Rx" },
    { "Ledipasvir-Sofosbuvir (Harvoni)", "Antiviral", "HCV genotype 1 treatment", "90/400mg", 17500.00f, "Rx" },
    { "Remdesivir (Veklury)", "Antiviral", "COVID-19 IV antiviral treatment", "100mg", 520.00f, "Rx" },
    { "Nirmatrelvir-Ritonavir (Paxlovid)", "Antiviral", "COVID-19 oral antiviral", "150/100mg", 185.00f, "Rx" },
    { "Molnupiravir (Lagevrio)", "Antiviral", "COVID-19 oral antiviral", "200mg", 145.00f, "Rx" },
    { "Ganciclovir (Cytovene)", "Antiviral", "CMV retinitis HIV treatment", "500mg", 245.00f, "Rx" },
    { "Valganciclovir (Valcyte)", "Antiviral", "CMV prevention transplant oral", "450mg", 285.00f, "Rx" },
    { "Fluconazole (Diflucan)", "Antifungal", "Candida and cryptococcal infections", "150mg", 18.00f, "Rx" },
    { "Itraconazole (Sporanox)", "Antifungal", "Systemic fungal infections azole", "100mg", 85.00f, "Rx" },
    { "Voriconazole (Vfend)", "Antifungal", "Aspergillus and serious fungi", "200mg", 145.00f, "Rx" },
    { "Posaconazole (Noxafil)", "Antifungal", "Prophylaxis and invasive fungi", "100mg", 185.00f, "Rx" },
    { "Isavuconazole (Cresemba)", "Antifungal", "Aspergillus and mucormycosis", "186mg", 245.00f, "Rx" },
    { "Amphotericin B (Fungizone)", "Antifungal", "Severe systemic fungal infections", "50mg", 185.00f, "Rx" },
    { "Liposomal Amphotericin (AmBisome)", "Antifungal", "Invasive fungi with reduced toxicity", "50mg", 285.00f, "Rx" },
    { "Caspofungin (Cancidas)", "Antifungal", "Echinocandin for candida/aspergillus", "50mg", 285.00f, "Rx" },
    { "Micafungin (Mycamine)", "Antifungal", "Echinocandin candida treatment", "100mg", 265.00f, "Rx" },
    { "Anidulafungin (Eraxis)", "Antifungal", "Echinocandin for candidiasis", "100mg", 275.00f, "Rx" },
    { "Terbinafine (Lamisil)", "Antifungal", "Oral antifungal for onychomycosis", "250mg", 38.00f, "Rx" },
    { "Griseofulvin (Grifulvin V)", "Antifungal", "Oral antifungal tinea infections", "500mg", 45.00f, "Rx" },
    { "Nystatin (Mycostatin)", "Antifungal", "Oral candida and mucosal infection", "500000 units", 22.00f, "Rx" },
    { "Cyclobenzaprine (Flexeril)", "Muscle Relaxant", "Acute muscle spasm relief", "10mg", 18.00f, "Rx" },
    { "Baclofen (Lioresal)", "Muscle Relaxant", "Spasticity and muscle spasm", "10mg", 16.00f, "Rx" },
    { "Tizanidine (Zanaflex)", "Muscle Relaxant", "Central alpha-2 agonist spasm", "4mg", 22.00f, "Rx" },
    { "Metaxalone (Skelaxin)", "Muscle Relaxant", "Musculoskeletal spasm relief", "800mg", 85.00f, "Rx" },
    { "Carisoprodol (Soma)", "Muscle Relaxant", "Short-term muscle spasm treatment", "350mg", 28.00f, "Rx" },
    { "Orphenadrine (Norflex)", "Muscle Relaxant", "Muscle spasm and pain", "100mg", 32.00f, "Rx" },
    { "Chlorzoxazone (Parafon Forte)", "Muscle Relaxant", "Musculoskeletal condition spasm", "500mg", 24.00f, "Rx" },
    { "Dantrolene (Dantrium)", "Muscle Relaxant", "Spasticity and malignant hyperthermia", "25mg", 45.00f, "Rx" },
    { "Botulinum Toxin A (Botox)", "Muscle Relaxant", "Cervical dystonia and spasticity", "100 units", 385.00f, "Rx" },
    { "Latanoprost (Xalatan)", "Eye/ENT", "Glaucoma prostaglandin eye drops", "0.005%", 35.00f, "Rx" },
    { "Timolol (Timoptic)", "Eye/ENT", "Glaucoma beta-blocker eye drops", "0.5%", 28.00f, "Rx" },
    { "Brimonidine (Alphagan P)", "Eye/ENT", "Glaucoma alpha-2 agonist drops", "0.1%", 42.00f, "Rx" },
    { "Dorzolamide (Trusopt)", "Eye/ENT", "Glaucoma carbonic anhydrase inhib", "2%", 38.00f, "Rx" },
    { "Ciprofloxacin Otic (Ciprodex)", "Eye/ENT", "Ear infection antibiotic drops", "0.3%", 45.00f, "Rx" },
    { "Ofloxacin Ophthalmic", "Eye/ENT", "Bacterial eye infection treatment", "0.3%", 28.00f, "Rx" },
    { "Moxifloxacin Ophthalmic (Vigamox)", "Eye/ENT", "Bacterial conjunctivitis drops", "0.5%", 42.00f, "Rx" },
    { "Prednisolone Acetate (Pred Forte)", "Eye/ENT", "Ocular inflammatory conditions", "1%", 35.00f, "Rx" },
    { "Cyclosporine Ophthalmic (Restasis)", "Eye/ENT", "Dry eye immunomodulator drops", "0.05%", 185.00f, "Rx" },
    { "Artificial Tears (Systane)", "Eye/ENT", "Dry eye lubricating drops", "0.4%", 11.99f, "OTC" },
    { "Pseudoephedrine (Sudafed)", "Eye/ENT", "Nasal decongestant oral", "30mg", 9.99f, "OTC" },
    { "Oxymetazoline (Afrin)", "Eye/ENT", "Nasal decongestant spray", "0.05%", 8.99f, "OTC" },
    { "Benzoyl Peroxide Wash", "Eye/ENT", "ENT cleansing antibacterial", "10%", 12.99f, "OTC" },
    { "Carbamide Peroxide (Debrox)", "Eye/ENT", "Ear wax removal drops", "6.5%", 7.99f, "OTC" },
    { "Naphazoline-Pheniramine (Visine-A)", "Eye/ENT", "Eye allergy and redness relief", "0.025%", 9.99f, "OTC" },
    { "Ranibizumab (Lucentis)", "Eye/ENT", "AMD anti-VEGF eye injection", "0.5mg", 1850.00f, "Rx" },
    { "Aflibercept (Eylea)", "Eye/ENT", "Wet AMD anti-VEGF injection", "2mg", 1950.00f, "Rx" },
    { "Epoetin Alfa (Epogen)", "Blood", "Anemia erythropoiesis stimulant", "10000 units", 285.00f, "Rx" },
    { "Darbepoetin (Aranesp)", "Blood", "Long-acting ESA for anemia", "25mcg", 385.00f, "Rx" },
    { "Filgrastim (Neupogen)", "Blood", "G-CSF for neutropenia", "300mcg", 485.00f, "Rx" },
    { "Pegfilgrastim (Neulasta)", "Blood", "Long-acting G-CSF neutropenia", "6mg", 3850.00f, "Rx" },
    { "Ferrous Sulfate", "Blood", "Iron deficiency anemia treatment", "325mg", 7.99f, "OTC" },
    { "Iron Dextran (INFeD)", "Blood", "IV iron for severe deficiency", "50mg/mL", 285.00f, "Rx" },
    { "Ferric Carboxymaltose (Injectafer)", "Blood", "IV iron repletion for IDA", "750mg/15mL", 485.00f, "Rx" },
    { "Hydroxyurea (Hydrea)", "Blood", "Sickle cell disease treatment", "500mg", 85.00f, "Rx" },
    { "Crizanlizumab (Adakveo)", "Blood", "Anti-P-selectin sickle cell", "100mg", 3850.00f, "Rx" },
    { "Voxelotor (Oxbryta)", "Blood", "HbS polymerization inhibitor SCD", "500mg", 2850.00f, "Rx" },
    { "Eltrombopag (Promacta)", "Blood", "Thrombopoietin for ITP", "50mg", 1850.00f, "Rx" },
    { "Romiplostim (Nplate)", "Blood", "Thrombopoietin agonist for ITP", "250mcg", 2850.00f, "Rx" },
    { "Phytonadione (Vitamin K)", "Blood", "Anticoagulant reversal vitamin K", "10mg", 18.00f, "Rx" },
    { "Protamine Sulfate", "Blood", "Heparin reversal agent", "50mg", 45.00f, "Rx" },
    { "Idarucizumab (Praxbind)", "Blood", "Dabigatran reversal agent", "5g", 3850.00f, "Rx" },
    { "Andexanet Alfa (Andexxa)", "Blood", "Factor Xa inhibitor reversal", "400mg", 18500.00f, "Rx" },
    { "Zolpidem (Ambien)", "Sleep", "Non-benzo hypnotic for insomnia", "10mg", 22.00f, "Rx" },
    { "Zaleplon (Sonata)", "Sleep", "Short-acting non-benzo hypnotic", "10mg", 28.00f, "Rx" },
    { "Eszopiclone (Lunesta)", "Sleep", "Non-benzo hypnotic for insomnia", "2mg", 32.00f, "Rx" },
    { "Ramelteon (Rozerem)", "Sleep", "Melatonin receptor agonist sleep", "8mg", 45.00f, "Rx" },
    { "Suvorexant (Belsomra)", "Sleep", "Orexin antagonist for insomnia", "10mg", 245.00f, "Rx" },
    { "Lemborexant (Dayvigo)", "Sleep", "Dual orexin receptor antagonist", "5mg", 255.00f, "Rx" },
    { "Doxepin (Silenor)", "Sleep", "Low-dose TCA for insomnia", "6mg", 85.00f, "Rx" },
    { "Melatonin (Natrol)", "Sleep", "Sleep onset and jet lag aid", "5mg", 8.99f, "OTC" },
    { "Diphenhydramine (ZzzQuil)", "Sleep", "OTC antihistamine sleep aid", "25mg", 9.99f, "OTC" },
    { "Doxylamine (Unisom)", "Sleep", "Antihistamine OTC sleep aid", "25mg", 8.99f, "OTC" },
    { "Temazepam (Restoril)", "Sleep", "Benzo short-term sleep aid", "15mg", 24.00f, "Rx" },
    { "Triazolam (Halcion)", "Sleep", "Ultra-short benzo hypnotic", "0.125mg", 28.00f, "Rx" },
    { "Desiccated Thyroid (NP Thyroid)", "Thyroid", "T3/T4 natural thyroid extract", "30mg", 26.00f, "Rx" },
    { "Propylthiouracil", "Thyroid", "Hyperthyroid pregnancy treatment", "100mg", 18.00f, "Rx" },
    { "Radioactive Iodine (I-131)", "Thyroid", "Hyperthyroidism ablation therapy", "5-15 mCi", 285.00f, "Rx" },
    { "Potassium Iodide (SSKI)", "Thyroid", "Pre-surgical thyroid preparation", "65mg", 24.00f, "Rx" },
    { "Alendronate (Fosamax)", "Bone", "Osteoporosis bisphosphonate weekly", "70mg", 22.00f, "Rx" },
    { "Risedronate (Actonel)", "Bone", "Osteoporosis bisphosphonate weekly", "35mg", 28.00f, "Rx" },
    { "Ibandronate (Boniva)", "Bone", "Osteoporosis monthly bisphosphonate", "150mg", 35.00f, "Rx" },
    { "Zoledronic Acid (Reclast)", "Bone", "Annual IV bisphosphonate osteoporosis", "5mg/100mL", 245.00f, "Rx" },
    { "Teriparatide (Forteo)", "Bone", "PTH analog anabolic bone builder", "20mcg/day", 2850.00f, "Rx" },
    { "Abaloparatide (Tymlos)", "Bone", "PTHrP analog for osteoporosis", "80mcg/day", 2650.00f, "Rx" },
    { "Denosumab (Prolia)", "Bone", "RANK-L inhibitor for osteoporosis", "60mg", 1185.00f, "Rx" },
    { "Romosozumab (Evenity)", "Bone", "Sclerostin inhibitor for osteoporosis", "210mg", 1850.00f, "Rx" },
    { "Calcium Carbonate (OsCal)", "Bone", "Calcium supplementation for bones", "1250mg", 13.99f, "OTC" },
    { "Raloxifene (Evista)", "Bone", "SERM for osteoporosis prevention", "60mg", 85.00f, "Rx" },
    { "Ethinyl Estradiol-Levonorgestrel", "Women's Health", "Combined OCP contraception", "30/150mcg", 28.00f, "Rx" },
    { "Ethinyl Estradiol-Norethindrone", "Women's Health", "Monophasic OCP contraception", "35/1mg", 26.00f, "Rx" },
    { "Ethinyl Estradiol-Drospirenone (Yaz)", "Women's Health", "OCP and PMDD treatment", "20/3mg", 32.00f, "Rx" },
    { "Levonorgestrel IUD (Mirena)", "Women's Health", "Long-acting hormonal IUD", "52mg", 845.00f, "Rx" },
    { "Etonogestrel Implant (Nexplanon)", "Women's Health", "3-year progestogen contraception", "68mg", 785.00f, "Rx" },
    { "Depot Medroxyprogesterone (Depo-Provera)", "Women's Health", "Progestin-only injectable", "150mg", 45.00f, "Rx" },
    { "Levonorgestrel (Plan B)", "Women's Health", "Emergency contraception", "1.5mg", 45.00f, "OTC" },
    { "Ulipristal Acetate (Ella)", "Women's Health", "Emergency contraception 5-day", "30mg", 55.00f, "Rx" },
    { "Mifepristone (Mifeprex)", "Women's Health", "Medical abortion abortifacient", "200mg", 85.00f, "Rx" },
    { "Misoprostol (Cytotec)", "Women's Health", "Cervical ripening and abortion", "200mcg", 42.00f, "Rx" },
    { "Clomiphene (Clomid)", "Women's Health", "Ovulation induction for fertility", "50mg", 45.00f, "Rx" },
    { "Letrozole (Femara)", "Women's Health", "Aromatase inhibitor ovulation induc", "2.5mg", 38.00f, "Rx" },
    { "Follitropin (Gonal-F)", "Women's Health", "FSH for ART ovarian stimulation", "75 IU", 285.00f, "Rx" },
    { "hCG (Ovidrel)", "Women's Health", "Ovulation trigger for ART", "250mcg", 145.00f, "Rx" },
    { "Leuprolide (Lupron Depot)", "Women's Health", "GnRH agonist endometriosis/fibroids", "3.75mg", 385.00f, "Rx" },
    { "Elagolix (Orilissa)", "Women's Health", "GnRH antagonist for endometriosis", "150mg", 845.00f, "Rx" },
    { "Norethindrone (Aygestin)", "Women's Health", "Progestin for endometriosis/DUB", "5mg", 45.00f, "Rx" },
    { "Spironolactone", "Women's Health", "Anti-androgen for PCOS/hirsutism", "100mg", 18.00f, "Rx" },
    { "Ospemifene (Osphena)", "Women's Health", "SERM for dyspareunia menopause", "60mg", 185.00f, "Rx" },
    { "Sildenafil (Viagra)", "Men's Health", "PDE5 inhibitor for erectile dysfunc", "50mg", 45.00f, "Rx" },
    { "Tadalafil (Cialis)", "Men's Health", "Daily or PRN PDE5i for ED", "10mg", 48.00f, "Rx" },
    { "Vardenafil (Levitra)", "Men's Health", "PDE5 inhibitor for ED", "10mg", 46.00f, "Rx" },
    { "Avanafil (Stendra)", "Men's Health", "Fast-onset PDE5 inhibitor for ED", "100mg", 52.00f, "Rx" },
    { "Finasteride (Proscar)", "Men's Health", "5-alpha reductase for BPH/AGA", "5mg", 22.00f, "Rx" },
    { "Dutasteride (Avodart)", "Men's Health", "Dual 5-ARI for BPH", "0.5mg", 35.00f, "Rx" },
    { "Tamsulosin (Flomax)", "Men's Health", "Alpha blocker for BPH symptoms", "0.4mg", 22.00f, "Rx" },
    { "Alfuzosin (Uroxatral)", "Men's Health", "Alpha blocker for BPH", "10mg", 28.00f, "Rx" },
    { "Doxazosin (Cardura)", "Men's Health", "Alpha blocker BPH and HTN", "4mg", 18.00f, "Rx" },
    { "Testosterone Enanthate", "Men's Health", "Injectable androgen replacement", "250mg/mL", 75.00f, "Rx" },
    { "Clomiphene Citrate (Clomid)", "Men's Health", "Male hypogonadism fertility", "50mg", 45.00f, "Rx" },
    { "hCG (Pregnyl)", "Men's Health", "Gonadotropin for male fertility", "10000 units", 165.00f, "Rx" },
    { "Cisplatin (Platinol)", "Oncology", "Platinum chemotherapy agent", "50mg", 485.00f, "Rx" },
    { "Carboplatin (Paraplatin)", "Oncology", "Platinum analog chemotherapy", "450mg", 285.00f, "Rx" },
    { "Oxaliplatin (Eloxatin)", "Oncology", "Platinum for colorectal cancer", "50mg", 385.00f, "Rx" },
    { "Paclitaxel (Taxol)", "Oncology", "Taxane chemotherapy agent", "100mg", 385.00f, "Rx" },
    { "Docetaxel (Taxotere)", "Oncology", "Taxane for breast and lung cancer", "80mg", 485.00f, "Rx" },
    { "Doxorubicin (Adriamycin)", "Oncology", "Anthracycline chemotherapy", "50mg", 285.00f, "Rx" },
    { "Cyclophosphamide (Cytoxan)", "Oncology", "Alkylating agent chemotherapy", "500mg", 185.00f, "Rx" },
    { "5-Fluorouracil (Adrucil)", "Oncology", "Pyrimidine antimetabolite chemo", "500mg", 145.00f, "Rx" },
    { "Gemcitabine (Gemzar)", "Oncology", "Nucleoside analog chemotherapy", "200mg", 245.00f, "Rx" },
    { "Irinotecan (Camptosar)", "Oncology", "Topoisomerase I inhibitor chemo", "100mg", 285.00f, "Rx" },
    { "Topotecan (Hycamtin)", "Oncology", "Topoisomerase I inhibitor", "4mg", 385.00f, "Rx" },
    { "Etoposide (Toposar)", "Oncology", "Topoisomerase II inhibitor", "100mg", 185.00f, "Rx" },
    { "Methotrexate (Otrexup)", "Oncology", "Antimetabolite cancer and RA", "25mg", 85.00f, "Rx" },
    { "Vincristine (Oncovin)", "Oncology", "Vinca alkaloid leukemia treatment", "1mg", 145.00f, "Rx" },
    { "Imatinib (Gleevec)", "Oncology", "BCR-ABL tyrosine kinase inhibitor", "400mg", 3850.00f, "Rx" },
    { "Dasatinib (Sprycel)", "Oncology", "2nd-gen BCR-ABL TKI for CML", "100mg", 4850.00f, "Rx" },
    { "Erlotinib (Tarceva)", "Oncology", "EGFR inhibitor NSCLC treatment", "150mg", 3850.00f, "Rx" },
    { "Osimertinib (Tagrisso)", "Oncology", "3rd-gen EGFR inhibitor NSCLC", "80mg", 5850.00f, "Rx" },
    { "Vemurafenib (Zelboraf)", "Oncology", "BRAF V600E inhibitor melanoma", "240mg", 4850.00f, "Rx" },
    { "Pembrolizumab (Keytruda)", "Oncology", "Anti-PD-1 immunotherapy", "200mg", 7850.00f, "Rx" },
    { "Nivolumab (Opdivo)", "Oncology", "Anti-PD-1 checkpoint inhibitor", "240mg", 7250.00f, "Rx" },
    { "Atezolizumab (Tecentriq)", "Oncology", "Anti-PD-L1 immunotherapy", "840mg", 6850.00f, "Rx" },
    { "Ipilimumab (Yervoy)", "Oncology", "Anti-CTLA-4 immunotherapy", "50mg", 8500.00f, "Rx" },
    { "Trastuzumab (Herceptin)", "Oncology", "HER2-targeted biologic breast CA", "440mg", 3850.00f, "Rx" },
    { "Bevacizumab (Avastin)", "Oncology", "Anti-VEGF biologic angiogenesis", "400mg", 2850.00f, "Rx" },
    { "Rituximab (Rituxan)", "Oncology", "Anti-CD20 for lymphoma and RA", "500mg", 3250.00f, "Rx" },
    { "Tamoxifen (Nolvadex)", "Oncology", "SERM for ER+ breast cancer", "20mg", 28.00f, "Rx" },
    { "Anastrozole (Arimidex)", "Oncology", "Aromatase inhibitor breast cancer", "1mg", 32.00f, "Rx" },
    { "Exemestane (Aromasin)", "Oncology", "Steroidal AI for breast cancer", "25mg", 42.00f, "Rx" },
    { "Leuprolide (Lupron)", "Oncology", "GnRH agonist prostate cancer", "7.5mg", 485.00f, "Rx" },
    { "Enzalutamide (Xtandi)", "Oncology", "Androgen receptor antagonist PCa", "40mg", 7850.00f, "Rx" },
    { "Abiraterone (Zytiga)", "Oncology", "CYP17 inhibitor for prostate cancer", "250mg", 4850.00f, "Rx" },
    { "Cyclosporine (Neoral)", "Immune", "Calcineurin inhibitor immunosuppressant", "100mg", 145.00f, "Rx" },
    { "Tacrolimus (Prograf)", "Immune", "Calcineurin inhibitor transplant", "1mg", 185.00f, "Rx" },
    { "Mycophenolate (CellCept)", "Immune", "Antiproliferative immunosuppressant", "500mg", 85.00f, "Rx" },
    { "Azathioprine (Imuran)", "Immune", "Purine analog immunosuppressant", "50mg", 45.00f, "Rx" },
    { "Sirolimus (Rapamune)", "Immune", "mTOR inhibitor transplant rejection", "1mg", 185.00f, "Rx" },
    { "Everolimus (Zortress)", "Immune", "mTOR inhibitor transplant and cancer", "0.75mg", 245.00f, "Rx" },
    { "Belimumab (Benlysta)", "Immune", "B-lymphocyte stimulator inhibitor SLE", "200mg", 1850.00f, "Rx" },
    { "Hydroxychloroquine (Plaquenil)", "Immune", "Antimalarial for lupus and RA", "200mg", 22.00f, "Rx" },
    { "Abatacept (Orencia)", "Immune", "T-cell costimulation blocker RA", "125mg", 1850.00f, "Rx" },
    { "Tocilizumab (Actemra)", "Immune", "IL-6 receptor inhibitor for RA", "162mg", 1650.00f, "Rx" },
    { "Baricitinib (Olumiant)", "Immune", "JAK1/2 inhibitor for RA", "4mg", 1450.00f, "Rx" },
    { "Tofacitinib (Xeljanz)", "Immune", "JAK inhibitor for RA and UC", "5mg", 1550.00f, "Rx" },
    { "Upadacitinib (Rinvoq)", "Immune", "Selective JAK1 inhibitor for RA", "15mg", 1650.00f, "Rx" },
    { "Intravenous Immunoglobulin (IVIG)", "Immune", "IgG replacement immunodeficiency", "10g/200mL", 1850.00f, "Rx" },
    { "Epinephrine (Adrenalin)", "Emergency", "Cardiac arrest and anaphylaxis IV", "1mg/mL", 45.00f, "Rx" },
    { "Atropine Sulfate", "Emergency", "Bradycardia and nerve agent antidote", "1mg/mL", 35.00f, "Rx" },
    { "Naloxone (Narcan)", "Emergency", "Opioid overdose reversal intranasal", "4mg", 38.00f, "Rx" },
    { "Flumazenil (Romazicon)", "Emergency", "Benzodiazepine reversal agent IV", "0.1mg/mL", 55.00f, "Rx" },
    { "Glucose 50% (D50)", "Emergency", "Hypoglycemia emergency IV dextrose", "25g/50mL", 18.00f, "Rx" },
    { "Glucagon (Baqsimi)", "Emergency", "Severe hypoglycemia nasal rescue", "3mg", 185.00f, "Rx" },
    { "Sodium Bicarbonate", "Emergency", "Metabolic acidosis and overdose", "50 mEq/50mL", 22.00f, "Rx" },
    { "Calcium Gluconate", "Emergency", "Hypocalcemia and hyperkalemia IV", "1g/10mL", 28.00f, "Rx" },
    { "Magnesium Sulfate", "Emergency", "Eclampsia and torsades treatment", "4g/50mL", 18.00f, "Rx" },
    { "Amiodarone IV", "Emergency", "VF/VT cardiac arrest IV treatment", "150mg/3mL", 65.00f, "Rx" },
    { "Norepinephrine (Levophed)", "Emergency", "Septic shock vasopressor IV", "4mg/4mL", 85.00f, "Rx" },
    { "Vasopressin", "Emergency", "Septic shock and cardiac arrest", "20 units/mL", 95.00f, "Rx" },
    { "Dopamine (Intropin)", "Emergency", "Cardiogenic shock vasopressor IV", "200mg/5mL", 45.00f, "Rx" },
    { "Dobutamine (Dobutrex)", "Emergency", "Cardiogenic shock inotrope IV", "250mg/20mL", 55.00f, "Rx" },
    { "Activated Charcoal (Actidose)", "Emergency", "Oral poisoning and overdose", "25g/120mL", 22.00f, "Rx" },
    { "Pralidoxime (2-PAM)", "Emergency", "Organophosphate antidote IV", "1g/20mL", 145.00f, "Rx" },
    { "Hydroxocobalamin (Cyanokit)", "Emergency", "Cyanide poisoning antidote IV", "5g/200mL", 1850.00f, "Rx" },
    { "Dextrose 10% (D10W)", "Emergency", "Neonatal hypoglycemia IV treatment", "250mL bag", 12.00f, "Rx" },
    { "Tranexamic Acid (TXA)", "Emergency", "Hemorrhage antifibrinolytic IV", "1g/10mL", 45.00f, "Rx" },
    { "Epinephrine (EpiPen)", "Emergency", "Anaphylaxis, cardiac arrest", "0.3mg", 650.00f, "Rx" },
    { "Atropine", "Emergency", "Bradycardia, organophosphate poisoning", "0.5mg", 18.00f, "Rx" },
    { "Adenosine", "Emergency", "SVT, paroxysmal supraventricular tachycardia", "6mg", 45.00f, "Rx" },
    { "Amiodarone", "Heart", "Ventricular arrhythmias", "200mg", 38.00f, "Rx" },
    { "Activated Charcoal", "Emergency", "Poisoning, drug overdose", "25g", 22.00f, "Rx" },
    { "Glucagon", "Emergency", "Severe hypoglycemia", "1mg", 180.00f, "Rx" },
    { "Heparin", "Blood", "DVT prophylaxis, anticoagulation", "5000u", 32.00f, "Rx" },
    { "Ferrous Gluconate", "Blood", "Iron deficiency anemia", "300mg", 7.99f, "OTC" },
    { "Folic Acid", "Vitamin", "Neural tube defect prevention, anemia", "1mg", 5.49f, "OTC" },
    { "Erythropoietin (Epogen)", "Blood", "Anemia from CKD, chemotherapy", "4000u", 320.00f, "Rx" },
    { "Hydroxyurea", "Blood", "Sickle cell disease, polycythemia", "500mg", 68.00f, "Rx" },
    { "Potassium Iodide", "Thyroid", "Radiation emergency, thyroid protection", "130mg", 11.00f, "OTC" },
    { "Conjugated Estrogen (Premarin)", "Women's Health", "Menopause symptoms, osteoporosis", "0.625mg", 55.00f, "Rx" },
    { "Methotrexate", "Women's Health", "Ectopic pregnancy, RA, psoriasis", "2.5mg", 35.00f, "Rx" },
    { "Norethindrone", "Women's Health", "Contraception, endometriosis", "0.35mg", 22.00f, "Rx" },
    { "Ethinyl Estradiol/Norgestimate", "Women's Health", "Oral contraceptive", "0.035mg", 35.00f, "Rx" },
    { "Dinoprostone (Cervidil)", "Women's Health", "Cervical ripening, labor induction", "10mg", 280.00f, "Rx" },
    { "Oxytocin (Pitocin)", "Women's Health", "Labor induction, postpartum bleeding", "10units", 25.00f, "Rx" },
    { "Lithium Carbonate", "Mental Health", "Bipolar disorder mood stabilizer", "300mg", 12.00f, "Rx" },
    { "Valproate (Depakote)", "Mental Health", "Bipolar, epilepsy, migraines", "500mg", 24.00f, "Rx" },
    { "Venlafaxine (Effexor)", "Mental Health", "Depression, anxiety, neuropathy", "75mg", 28.00f, "Rx" },
    { "Melatonin", "Sleep", "Insomnia, circadian rhythm disorders", "5mg", 8.99f, "OTC" },
    { "Phenobarbital", "Neurological", "Epilepsy, sedation", "30mg", 12.00f, "Rx" },
    { "Levodopa/Carbidopa (Sinemet)", "Neurological", "Parkinson disease", "25/100mg", 32.00f, "Rx" },
    { "Baclofen", "Muscle Relaxant", "Muscle spasticity, MS", "10mg", 14.00f, "Rx" },
    { "Diazepam (muscle)", "Muscle Relaxant", "Muscle spasm, spasticity", "5mg", 12.00f, "Rx" },
    { "Ribavirin", "Antiviral", "Hepatitis C, RSV", "200mg", 85.00f, "Rx" },
    { "Sofosbuvir (Sovaldi)", "Antiviral", "Hepatitis C (HCV)", "400mg", 84000.00f, "Rx" },
    { "Tenofovir (Viread)", "Antiviral", "HIV, hepatitis B", "300mg", 68.00f, "Rx" },
    { "Emtricitabine/Tenofovir (Truvada)", "Antiviral", "HIV PrEP, antiretroviral therapy", "200/300mg", 2000.00f, "Rx" },
    { "Atazanavir (Reyataz)", "Antiviral", "HIV protease inhibitor", "300mg", 2800.00f, "Rx" },
    { "Clotrimazole (Lotrimin)", "Antifungal", "Athlete's foot, ringworm, candidiasis", "1%", 8.99f, "OTC" },
    { "Miconazole (Monistat)", "Antifungal", "Vaginal yeast infection, athlete's foot", "2%", 12.99f, "OTC" },
    { "Salbutamol/Albuterol (Ventolin)", "Respiratory", "Acute bronchospasm, asthma rescue", "90mcg/puff", 32.00f, "Rx" },
    { "Fluticasone (Flovent)", "Respiratory", "Asthma inhaled corticosteroid", "110mcg/puff", 220.00f, "Rx" },
    { "Fluticasone/Salmeterol (Advair)", "Respiratory", "Asthma, COPD combination inhaler", "250/50mcg", 420.00f, "Rx" },
    { "Budesonide/Formoterol (Symbicort)", "Respiratory", "Asthma, COPD combination inhaler", "160/4.5mcg", 390.00f, "Rx" },
    { "Theophylline", "Respiratory", "Asthma, COPD bronchodilator", "200mg", 18.00f, "Rx" },
    { "Dextromethorphan (Robitussin DM)", "Respiratory", "Cough suppressant", "15mg", 7.99f, "OTC" },
    { "Prednisolone", "Respiratory", "Asthma exacerbation, anti-inflammatory", "5mg", 10.00f, "Rx" },
    { "Prednisone", "Immune", "Inflammation, autoimmune disease, asthma", "10mg", 8.00f, "Rx" },
    { "Methylprednisolone (Solu-Medrol)", "Immune", "Severe inflammation, MS relapse", "40mg", 22.00f, "Rx" },
    { "Dexamethasone", "Immune", "Inflammation, croup, cerebral edema", "0.5mg", 12.00f, "Rx" },
    { "Hydrocortisone", "Immune", "Adrenal insufficiency, inflammation", "20mg", 10.00f, "Rx" },
    { "Methotrexate (RA)", "Immune", "Rheumatoid arthritis, psoriasis", "7.5mg", 18.00f, "Rx" },
    { "Etanercept (Enbrel)", "Immune", "Rheumatoid arthritis, psoriasis", "50mg", 5800.00f, "Rx" },
    { "Benzoyl Peroxide (Benzac)", "Skin", "Acne, bacterial skin infections", "5%", 9.99f, "OTC" },
    { "Clindamycin topical", "Skin", "Acne topical antibiotic", "1%", 18.00f, "Rx" },
    { "Hydrocortisone cream", "Skin", "Eczema, contact dermatitis, itching", "1%", 6.99f, "OTC" },
    { "Triamcinolone (Kenalog)", "Skin", "Severe eczema, psoriasis", "0.1%", 14.00f, "Rx" },
    { "Tacrolimus ointment (Protopic)", "Skin", "Atopic dermatitis, eczema", "0.1%", 38.00f, "Rx" },
    { "Acyclovir cream (Zovirax)", "Skin", "Cold sores, herpes labialis", "5%", 14.99f, "OTC" },
    { "Permethrin (Elimite)", "Skin", "Scabies, head lice", "5%", 18.00f, "Rx" },
    { "Ketoconazole shampoo", "Skin", "Dandruff, seborrheic dermatitis", "2%", 16.99f, "OTC" },
    { "Silver Sulfadiazine (Silvadene)", "Skin", "Burn wound infections", "1%", 28.00f, "Rx" },
    { "Brimonidine (Alphagan)", "Eye/ENT", "Glaucoma, ocular hypertension", "0.2%", 35.00f, "Rx" },
    { "Timolol eye drops (Timoptic)", "Eye/ENT", "Glaucoma, ocular hypertension", "0.5%", 22.00f, "Rx" },
    { "Prednisolone eye drops", "Eye/ENT", "Eye inflammation, post-surgery", "1%", 18.00f, "Rx" },
    { "Ciprofloxacin eye drops", "Eye/ENT", "Bacterial conjunctivitis, eye infection", "0.3%", 14.00f, "Rx" },
    { "Tobramycin eye drops", "Eye/ENT", "Bacterial eye infection, conjunctivitis", "0.3%", 16.00f, "Rx" },
    { "Ketotifen eye drops (Zaditor)", "Eye/ENT", "Allergic conjunctivitis", "0.025%", 12.99f, "OTC" },
    { "Amoxicillin/Clavulanate (Augmentin)", "Antibiotic", "Resistant bacterial infections", "875/125mg", 28.00f, "Rx" },
    { "Vancomycin", "Antibiotic", "MRSA, C. difficile, severe infections", "125mg", 45.00f, "Rx" },
    { "Trimethoprim/Sulfamethoxazole (Bactrim)", "Antibiotic", "UTI, MRSA, Pneumocystis", "160/800mg", 10.00f, "Rx" },
    { "Tigecycline (Tygacil)", "Antibiotic", "Complicated infections, tetracycline class", "50mg", 120.00f, "Rx" },
    { "Semaglutide oral (Rybelsus)", "Diabetes", "Type 2 diabetes, GLP-1 oral", "7mg", 780.00f, "Rx" },
    { "Letrozole (Femara-onco)", "Oncology", "Breast cancer aromatase inhibitor", "2.5mg", 52.00f, "Rx" },
    { "Cisplatin", "Oncology", "Testicular, ovarian, bladder cancer", "50mg", 45.00f, "Rx" },
    { "Carboplatin", "Oncology", "Ovarian, lung, head and neck cancer", "450mg", 85.00f, "Rx" },
    { "Fluorouracil (5-FU)", "Oncology", "Colorectal, breast, skin cancer", "500mg", 32.00f, "Rx" },
    { "Capecitabine (Xeloda)", "Oncology", "Colorectal, breast cancer oral 5-FU", "500mg", 280.00f, "Rx" },
    { "Omega-3 Fatty Acids (Fish Oil)", "Vitamin", "Heart health, triglyceride reduction", "1000mg", 14.99f, "OTC" },
    { "Vitamin K2 (MK-7)", "Vitamin", "Bone health, calcium regulation", "100mcg", 18.99f, "OTC" },
    { "Biotin (Vitamin B7)", "Vitamin", "Hair, nail growth, metabolism", "5000mcg", 12.99f, "OTC" },
    { "Niacin (Vitamin B3)", "Vitamin", "Cholesterol, pellagra, energy metabolism", "500mg", 10.99f, "OTC" },
    { "Riboflavin (Vitamin B2)", "Vitamin", "Energy metabolism, migraine prevention", "100mg", 8.99f, "OTC" },
    { "Thiamine (Vitamin B1)", "Vitamin", "Beriberi, Wernicke encephalopathy", "100mg", 7.99f, "OTC" },
    { "Pantothenic Acid (B5)", "Vitamin", "Metabolism, wound healing", "500mg", 9.49f, "OTC" },
    { "Alpha Lipoic Acid", "Vitamin", "Antioxidant, neuropathy, diabetes", "600mg", 19.99f, "OTC" },
    { "Lactulose", "Digestive", "Constipation, hepatic encephalopathy", "10g/15mL", 16.00f, "Rx" },
    { "Senna (Senokot)", "Digestive", "Constipation, stimulant laxative", "8.6mg", 7.99f, "OTC" },
    { "Docusate (Colace)", "Digestive", "Stool softener, constipation", "100mg", 8.99f, "OTC" },
    { "Simethicone (Gas-X)", "Digestive", "Gas, bloating, flatulence", "125mg", 6.99f, "OTC" },
    { "Calcium Polycarbophil (FiberCon)", "Digestive", "IBS, constipation, bulk laxative", "625mg", 11.99f, "OTC" },
    { "Mesalamine (Asacol)", "Digestive", "Ulcerative colitis, Crohn disease", "400mg", 280.00f, "Rx" },
    { "Infliximab (digestive)", "Digestive", "Crohn disease, ulcerative colitis", "100mg", 1850.00f, "Rx" },
    { "Ezetimibe (Zetia)", "Heart", "High cholesterol, cholesterol absorption", "10mg", 32.00f, "Rx" },
    { "Evolocumab (Repatha)", "Heart", "High cholesterol, PCSK9 inhibitor", "140mg", 580.00f, "Rx" },
    { "Hydrochlorothiazide (HCTZ)", "Heart", "Hypertension, edema, diuretic", "25mg", 8.00f, "Rx" },
    { "Chlorthalidone", "Heart", "Hypertension, preferred thiazide diuretic", "25mg", 10.00f, "Rx" },
    { "Ivermectin", "Antibiotic", "Parasitic infections, scabies, onchocerciasis", "3mg", 18.00f, "Rx" },
    { "Albendazole (Albenza)", "Antibiotic", "Intestinal worms, tapeworm", "200mg", 22.00f, "Rx" },
    { "Mebendazole (Emverm)", "Antibiotic", "Pinworm, roundworm, hookworm", "100mg", 18.00f, "Rx" },
    { "Chloroquine (Aralen)", "Antibiotic", "Malaria prophylaxis and treatment", "250mg", 16.00f, "Rx" },
    { "Hydroxychloroquine (anti-malarial)", "Antibiotic", "Malaria, lupus, rheumatoid arthritis", "200mg", 14.00f, "Rx" },
    { "Atovaquone/Proguanil (Malarone)", "Antibiotic", "Malaria prophylaxis and treatment", "250/100mg", 145.00f, "Rx" },
    { "Pyrimethamine (Daraprim)", "Antibiotic", "Toxoplasmosis, malaria", "25mg", 1200.00f, "Rx" },
    { "Bupropion (Wellbutrin)", "Mental Health", "Depression, seasonal affective, ADHD", "150mg", 28.00f, "Rx" },
    { "Paliperidone (Invega)", "Mental Health", "Schizophrenia, schizoaffective disorder", "3mg", 45.00f, "Rx" },
    { "Buprenorphine/Naloxone (Suboxone)", "Mental Health", "Opioid use disorder treatment", "8/2mg", 95.00f, "Rx" },
    { "Varenicline (Chantix)", "Respiratory", "Smoking cessation", "0.5mg", 220.00f, "Rx" },
    { "Nicotine Patch (Nicoderm)", "Respiratory", "Smoking cessation NRT", "14mg", 38.99f, "OTC" },
    { "Nicotine Gum (Nicorette)", "Respiratory", "Smoking cessation NRT", "2mg", 28.99f, "OTC" },
    { "Orlistat (Alli)", "Digestive", "Obesity, weight management", "60mg", 52.99f, "OTC" },
    { "Phentermine (Adipex)", "Digestive", "Obesity, short-term weight loss", "37.5mg", 32.00f, "Rx" },
    { "Semaglutide (Wegovy)", "Digestive", "Obesity, chronic weight management GLP-1", "2.4mg", 1350.00f, "Rx" },
    { "Tirzepatide (Mounjaro)", "Diabetes", "Type 2 diabetes, weight loss GIP/GLP-1", "5mg", 1050.00f, "Rx" },
    { "Hydroxyzine (Vistaril)", "Allergy", "Anxiety, allergic pruritus, sedation", "25mg", 12.00f, "Rx" },
    { "Cyproheptadine (Periactin)", "Allergy", "Allergies, appetite stimulation, migraine", "4mg", 14.00f, "Rx" },
    { "Cromolyn (NasalCrom)", "Allergy", "Allergic rhinitis prevention", "5.2mg/spray", 16.99f, "OTC" },
    { "Ipratropium nasal (Atrovent)", "Allergy", "Rhinorrhea, common cold, rhinitis", "0.03%", 28.00f, "Rx" },
    { "Letermovir (Prevymis)", "Antiviral", "CMV prophylaxis in transplant patients", "480mg", 8800.00f, "Rx" },
    { "Paxlovid (Nirmatrelvir/Ritonavir)", "Antiviral", "COVID-19 mild to moderate treatment", "300/100mg", 1390.00f, "Rx" },
    { "Ertugliflozin (Steglatro)", "Diabetes", "Type 2 diabetes, SGLT2 inhibitor", "5mg", 370.00f, "Rx" },
    { "Alogliptin (Nesina)", "Diabetes", "Type 2 diabetes, DPP-4 inhibitor", "25mg", 290.00f, "Rx" },
    { "Nateglinide (Starlix)", "Diabetes", "Type 2 diabetes, meglitinide", "60mg", 38.00f, "Rx" },
    { "Pramlintide (Symlin)", "Diabetes", "Type 1 and 2 diabetes adjunct", "60mcg", 180.00f, "Rx" },
    { "Insulin NPH", "Diabetes", "Intermediate-acting insulin", "100u/mL", 55.00f, "Rx" },
    { "Sacubitril/Valsartan (Entresto)", "Heart", "Heart failure with reduced ejection fraction", "24/26mg", 480.00f, "Rx" },
    { "Vericiguat (Verquvo)", "Heart", "Chronic heart failure, sGC stimulator", "2.5mg", 580.00f, "Rx" },
    { "Isosorbide Mononitrate", "Heart", "Stable angina, prophylaxis", "30mg", 18.00f, "Rx" },
    { "Ranolazine (Ranexa)", "Heart", "Chronic angina, anti-ischemic", "500mg", 220.00f, "Rx" },
    { "Dronedarone (Multaq)", "Heart", "Atrial fibrillation rhythm control", "400mg", 340.00f, "Rx" },
    { "Propafenone (Rhythmol)", "Heart", "Atrial fibrillation, ventricular arrhythmias", "150mg", 52.00f, "Rx" },
    { "Dofetilide (Tikosyn)", "Heart", "Atrial fibrillation maintenance", "125mcg", 180.00f, "Rx" },
    { "Alirocumab (Praluent)", "Blood", "High LDL cholesterol, PCSK9 inhibitor", "75mg", 580.00f, "Rx" },
    { "Inclisiran (Leqvio)", "Blood", "High LDL, siRNA PCSK9 inhibitor, twice yearly", "284mg", 3200.00f, "Rx" },
    { "Bempedoic Acid (Nexletol)", "Blood", "High LDL, ATP-citrate lyase inhibitor", "180mg", 280.00f, "Rx" },
    { "Icosapent Ethyl (Vascepa)", "Blood", "Hypertriglyceridemia, cardiovascular risk", "4g/day", 420.00f, "Rx" },
    { "Lomitapide (Juxtapid)", "Blood", "Familial hypercholesterolemia, MTP inhibitor", "5mg", 18000.00f, "Rx" },
    { "Colchicine (Colcrys)", "Pain Relief", "Gout flares, pericarditis, FMF", "0.6mg", 45.00f, "Rx" },
    { "Febuxostat (Uloric)", "Pain Relief", "Gout, chronic hyperuricemia", "40mg", 85.00f, "Rx" },
    { "Pegloticase (Krystexxa)", "Pain Relief", "Refractory gout, urate-lowering", "8mg", 6500.00f, "Rx" },
    { "Probenecid", "Pain Relief", "Gout, uricosuric agent", "500mg", 18.00f, "Rx" },
    { "Etodolac", "Pain Relief", "Osteoarthritis, rheumatoid arthritis", "400mg", 20.00f, "Rx" },
    { "Ketoprofen", "Pain Relief", "Arthritis, pain, menstrual cramps", "75mg", 16.00f, "Rx" },
    { "Naproxen Sodium (Aleve)", "Pain Relief", "Pain, fever, inflammation", "220mg", 8.99f, "OTC" },
    { "Hydromorphone (Dilaudid)", "Pain Relief", "Severe pain, opioid analgesic", "2mg", 28.00f, "Rx" },
    { "Oxymorphone (Opana)", "Pain Relief", "Severe chronic pain, opioid", "5mg", 95.00f, "Rx" },
    { "Methadone (Dolophine)", "Pain Relief", "Opioid use disorder, chronic pain", "5mg", 12.00f, "Rx" },
    { "Ketamine (Ketalar)", "Neurological", "Anesthesia, treatment-resistant depression", "500mg", 280.00f, "Rx" },
    { "Esketamine (Spravato)", "Mental Health", "Treatment-resistant depression, nasal spray", "56mg", 800.00f, "Rx" },
    { "Lumateperone (Caplyta)", "Mental Health", "Schizophrenia, bipolar depression", "42mg", 140.00f, "Rx" },
    { "Perphenazine", "Mental Health", "Schizophrenia, nausea", "4mg", 22.00f, "Rx" },
    { "Nefazodone", "Mental Health", "Depression, anxiety", "100mg", 28.00f, "Rx" },
    { "Pimavanserin (Nuplazid)", "Mental Health", "Parkinson's disease psychosis", "34mg", 2800.00f, "Rx" },
    { "Brexanolone (Zulresso)", "Mental Health", "Postpartum depression, GABA modulator", "60hr infusion", 34000.00f, "Rx" },
    { "Zuranolone (Zurzuvae)", "Mental Health", "Major depressive disorder, postpartum", "50mg", 720.00f, "Rx" },
    { "Eptinezumab (Vyepti)", "Neurological", "Migraine prevention, IV CGRP antibody", "100mg qrtly", 1200.00f, "Rx" },
    { "Ubrogepant (Ubrelvy)", "Neurological", "Acute migraine, CGRP receptor antagonist", "50mg", 75.00f, "Rx" },
    { "Rimegepant (Nurtec)", "Neurological", "Acute migraine and prevention, CGRP antagonist", "75mg", 80.00f, "Rx" },
    { "Lasmiditan (Reyvow)", "Neurological", "Acute migraine, 5-HT1F agonist", "50mg", 68.00f, "Rx" },
    { "Frovatriptan (Frova)", "Neurological", "Acute migraine, menstrual migraine", "2.5mg", 38.00f, "Rx" },
    { "Naratriptan (Amerge)", "Neurological", "Acute migraine, gentler onset", "1mg", 28.00f, "Rx" },
    { "Zolmitriptan (Zomig)", "Neurological", "Acute migraine, nasal and oral", "2.5mg", 30.00f, "Rx" },
    { "Rotigotine (Neupro)", "Neurological", "Parkinson's disease, RLS, transdermal patch", "2mg", 240.00f, "Rx" },
    { "Safinamide (Xadago)", "Neurological", "Parkinson's disease adjunct, MAO-B inhibitor", "50mg", 520.00f, "Rx" },
    { "Opicapone (Ongentys)", "Neurological", "Parkinson's disease, COMT inhibitor", "50mg", 580.00f, "Rx" },
    { "Amantadine (Gocovri)", "Neurological", "Parkinson's dyskinesia, influenza", "100mg", 38.00f, "Rx" },
    { "Deutetrabenazine (Austedo)", "Neurological", "Tardive dyskinesia, Huntington chorea", "6mg", 2800.00f, "Rx" },
    { "Valbenazine (Ingrezza)", "Neurological", "Tardive dyskinesia, VMAT2 inhibitor", "40mg", 3200.00f, "Rx" },
    { "Riluzole (Rilutek)", "Neurological", "ALS, glutamate blocker", "50mg", 380.00f, "Rx" },
    { "Edaravone (Radicava)", "Neurological", "ALS, free radical scavenger", "60mg", 15000.00f, "Rx" },
    { "Nusinersen (Spinraza)", "Neurological", "Spinal muscular atrophy, antisense oligonucleotide", "12mg", 125000.00f, "Rx" },
    { "Lecanemab (Leqembi)", "Neurological", "Early Alzheimer's, amyloid antibody", "10mg/kg biweekly", 26500.00f, "Rx" },
    { "Certolizumab (Cimzia)", "Immune", "RA, Crohn's, anti-TNF PEGylated", "200mg", 2400.00f, "Rx" },
    { "Golimumab (Simponi)", "Immune", "RA, psoriatic arthritis, anti-TNF monthly", "50mg/mo", 2600.00f, "Rx" },
    { "Sarilumab (Kevzara)", "Immune", "Rheumatoid arthritis, IL-6 receptor antagonist", "200mg/2wk", 1900.00f, "Rx" },
    { "Bimekizumab (Bimzelx)", "Immune", "Plaque psoriasis, IL-17A and F inhibitor", "320mg", 5200.00f, "Rx" },
    { "Guselkumab (Tremfya)", "Immune", "Psoriasis, psoriatic arthritis, IL-23 blocker", "100mg q8wk", 3800.00f, "Rx" },
    { "Risankizumab (Skyrizi)", "Immune", "Psoriasis, psoriatic arthritis, Crohn's, IL-23 p19", "150mg q12wk", 4200.00f, "Rx" },
    { "Tildrakizumab (Ilumya)", "Immune", "Plaque psoriasis, IL-23 inhibitor", "100mg q12wk", 3500.00f, "Rx" },
    { "Apremilast (Otezla)", "Immune", "Psoriatic arthritis, plaque psoriasis, PDE4 inhibitor", "30mg", 280.00f, "Rx" },
    { "Anifrolumab (Saphnelo)", "Immune", "Systemic lupus erythematosus, type I IFN blocker", "300mg q4wk", 3800.00f, "Rx" },
    { "Pirfenidone (Esbriet)", "Respiratory", "Idiopathic pulmonary fibrosis", "801mg", 850.00f, "Rx" },
    { "Nintedanib (Ofev)", "Respiratory", "Idiopathic pulmonary fibrosis, ILD", "150mg", 980.00f, "Rx" },
    { "Ivacaftor (Kalydeco)", "Respiratory", "Cystic fibrosis, CFTR potentiator", "150mg", 22000.00f, "Rx" },
    { "Elexacaftor/Tezacaftor/Ivacaftor (Trikafta)", "Respiratory", "Cystic fibrosis, F508del mutation", "2+1 tab combo", 28000.00f, "Rx" },
    { "Treprostinil (Remodulin)", "Heart", "Pulmonary arterial hypertension", "1.25ng/kg/min", 8500.00f, "Rx" },
    { "Bosentan (Tracleer)", "Heart", "Pulmonary arterial hypertension, ET receptor blocker", "62.5mg", 3200.00f, "Rx" },
    { "Ambrisentan (Letairis)", "Heart", "Pulmonary arterial hypertension, ERA", "5mg", 3800.00f, "Rx" },
    { "Riociguat (Adempas)", "Heart", "Pulmonary hypertension, sGC stimulator", "1mg", 2200.00f, "Rx" },
    { "Selexipag (Uptravi)", "Heart", "Pulmonary arterial hypertension, prostacyclin receptor", "200mcg", 5200.00f, "Rx" },
    { "Nilotinib (Tasigna)", "Oncology", "CML, second-line BCR-ABL TKI", "300mg", 950.00f, "Rx" },
    { "Ponatinib (Iclusig)", "Oncology", "CML, T315I mutation, BCR-ABL TKI", "45mg", 18000.00f, "Rx" },
    { "Gefitinib (Iressa)", "Oncology", "NSCLC, EGFR mutation, TKI", "250mg", 520.00f, "Rx" },
    { "Afatinib (Gilotrif)", "Oncology", "NSCLC, EGFR/HER2 TKI", "40mg", 550.00f, "Rx" },
    { "Alectinib (Alecensa)", "Oncology", "ALK+ NSCLC, CNS penetrant TKI", "150mg", 4800.00f, "Rx" },
    { "Crizotinib (Xalkori)", "Oncology", "ALK+ NSCLC, MET/ROS1, TKI", "250mg", 4200.00f, "Rx" },
    { "Lorlatinib (Lorbrena)", "Oncology", "ALK+ NSCLC, third-generation TKI", "100mg", 6500.00f, "Rx" },
    { "Sorafenib (Nexavar)", "Oncology", "Hepatocellular, RCC, thyroid cancer, multi-kinase TKI", "200mg", 2800.00f, "Rx" },
    { "Sunitinib (Sutent)", "Oncology", "RCC, GIST, multi-targeted TKI", "25mg", 3800.00f, "Rx" },
    { "Pazopanib (Votrient)", "Oncology", "RCC, soft tissue sarcoma, VEGFR TKI", "400mg", 3200.00f, "Rx" },
    { "Cabozantinib (Cabometyx)", "Oncology", "RCC, HCC, thyroid cancer, VEGFR/MET/AXL TKI", "60mg", 5200.00f, "Rx" },
    { "Lenvatinib (Lenvima)", "Oncology", "Thyroid, RCC, HCC, VEGFR TKI", "10mg", 4800.00f, "Rx" },
    { "Venetoclax (Venclexta)", "Oncology", "CLL, AML, BCL-2 inhibitor", "100mg", 5800.00f, "Rx" },
    { "Ibrutinib (Imbruvica)", "Oncology", "CLL, MCL, BTK inhibitor", "420mg", 14000.00f, "Rx" },
    { "Acalabrutinib (Calquence)", "Oncology", "CLL, MCL, second-gen BTK inhibitor", "100mg", 12000.00f, "Rx" },
    { "Zanubrutinib (Brukinsa)", "Oncology", "CLL, MCL, Waldenstrom, BTK inhibitor", "160mg", 11000.00f, "Rx" },
    { "Bortezomib (Velcade)", "Oncology", "Multiple myeloma, MCL, proteasome inhibitor", "1.3mg/m2", 850.00f, "Rx" },
    { "Carfilzomib (Kyprolis)", "Oncology", "Multiple myeloma, next-gen proteasome inhibitor", "20mg/m2", 2200.00f, "Rx" },
    { "Lenalidomide (Revlimid)", "Oncology", "Multiple myeloma, MDS, immunomodulator", "25mg", 700.00f, "Rx" },
    { "Pomalidomide (Pomalyst)", "Oncology", "Relapsed multiple myeloma, IMiD", "4mg", 880.00f, "Rx" },
    { "Ruxolitinib (Jakafi)", "Oncology", "Myelofibrosis, polycythemia vera, JAK1/2 inhibitor", "20mg", 9800.00f, "Rx" },
    { "Durvalumab (Imfinzi)", "Oncology", "NSCLC, biliary cancer, PD-L1 inhibitor", "1500mg q4wk", 9200.00f, "Rx" },
    { "Pertuzumab (Perjeta)", "Oncology", "HER2+ breast cancer, HER2 dimerization inhibitor", "840mg load", 3400.00f, "Rx" },
    { "Cetuximab (Erbitux)", "Oncology", "Colorectal, head/neck cancer, EGFR antibody", "400mg/m2 load", 2800.00f, "Rx" },
    { "Obinutuzumab (Gazyva)", "Oncology", "CLL, follicular lymphoma, anti-CD20", "1000mg", 3200.00f, "Rx" },
    { "Palbociclib (Ibrance)", "Oncology", "HR+/HER2- breast cancer, CDK4/6 inhibitor", "125mg", 4800.00f, "Rx" },
    { "Ribociclib (Kisqali)", "Oncology", "HR+ breast cancer, CDK4/6 inhibitor", "600mg", 4500.00f, "Rx" },
    { "Abemaciclib (Verzenio)", "Oncology", "HR+ breast cancer, CDK4/6 inhibitor continuous", "150mg", 4200.00f, "Rx" },
    { "Olaparib (Lynparza)", "Oncology", "BRCA+ breast, ovarian, pancreatic cancer, PARP inhibitor", "300mg", 5800.00f, "Rx" },
    { "Niraparib (Zejula)", "Oncology", "Ovarian cancer maintenance, PARP inhibitor", "300mg", 5200.00f, "Rx" },
    { "Dabrafenib (Tafinlar)", "Oncology", "BRAF V600 melanoma, NSCLC, BRAF inhibitor", "150mg", 5200.00f, "Rx" },
    { "Trametinib (Mekinist)", "Oncology", "BRAF-mutant melanoma, MEK inhibitor", "2mg", 4500.00f, "Rx" },
    { "Pemetrexed (Alimta)", "Oncology", "NSCLC, mesothelioma, antifolate chemotherapy", "500mg/m2", 1200.00f, "Rx" },
    { "Etoposide (VP-16)", "Oncology", "Lung, testicular cancer, topoisomerase II inhibitor", "50mg", 45.00f, "Rx" },
    { "Vinorelbine (Navelbine)", "Oncology", "NSCLC, breast cancer, vinca alkaloid", "25mg/m2", 380.00f, "Rx" },
    { "Cabazitaxel (Jevtana)", "Oncology", "Prostate cancer, taxane chemotherapy", "25mg/m2", 5800.00f, "Rx" },
    { "Darolutamide (Nubeqa)", "Oncology", "Non-metastatic prostate cancer, AR antagonist", "600mg", 7800.00f, "Rx" },
    { "Nirmatrelvir/Ritonavir (Paxlovid)", "Antiviral", "COVID-19, protease inhibitor combination", "300/100mg", 530.00f, "Rx" },
    { "Ledipasvir/Sofosbuvir (Harvoni)", "Antiviral", "Hepatitis C genotype 1, NS5A/NS5B inhibitor combo", "90/400mg", 1100.00f, "Rx" },
    { "Bictegravir/FTC/TAF (Biktarvy)", "Antiviral", "HIV-1 infection, single-tablet INI/NRTI regimen", "50/200/25mg", 3800.00f, "Rx" },
    { "Darunavir (Prezista)", "Antiviral", "HIV-1, boosted protease inhibitor", "800mg", 1200.00f, "Rx" },
    { "Rilpivirine (Edurant)", "Antiviral", "HIV-1, NNRTI for treatment-naive patients", "25mg", 1500.00f, "Rx" },
    { "Doravirine (Pifeltro)", "Antiviral", "HIV-1 NNRTI for treatment-naive", "100mg", 1600.00f, "Rx" },
    { "Maraviroc (Selzentry)", "Antiviral", "HIV-1 CCR5-tropic, CCR5 antagonist", "300mg", 1400.00f, "Rx" },
    { "Cefpodoxime (Vantin)", "Antibiotic", "URI, sinusitis, UTI, third-gen cephalosporin", "100mg", 25.00f, "Rx" },
    { "Cefadroxil (Duricef)", "Antibiotic", "Skin, throat, UTI, first-gen cephalosporin", "500mg", 18.00f, "Rx" },
    { "Daptomycin (Cubicin)", "Antibiotic", "MRSA skin infections, bacteremia, lipopeptide", "4mg/kg", 120.00f, "Rx" },
    { "Fidaxomicin (Dificid)", "Antibiotic", "C. difficile infection, macrolide antibiotic", "200mg", 3200.00f, "Rx" },
    { "Trimethoprim (Primsol)", "Antibiotic", "UTI, Pneumocystis prophylaxis", "100mg", 12.00f, "Rx" },
    { "Griseofulvin (Grifulvin)", "Antifungal", "Tinea capitis, dermatophyte infections, oral", "500mg", 18.00f, "Rx" },
    { "Ciclopirox (Penlac)", "Antifungal", "Onychomycosis, nail lacquer antifungal", "8%", 28.00f, "Rx" },
    { "Butenafine (Lotrimin Ultra)", "Antifungal", "Athlete's foot, tinea, benzylamine antifungal", "1%", 9.99f, "OTC" },
    { "Artemether-Lumefantrine (Coartem)", "Antibiotic", "Uncomplicated malaria treatment", "20/120mg", 45.00f, "Rx" },
    { "Tafenoquine (Krintafel)", "Antibiotic", "P. vivax malaria radical cure, 8-aminoquinoline", "300mg", 120.00f, "Rx" },
    { "Ivermectin (Stromectol)", "Antibiotic", "Strongyloidiasis, scabies, river blindness", "3mg", 12.00f, "Rx" },
    { "Praziquantel (Biltricide)", "Antibiotic", "Schistosomiasis, tapeworm, trematode infections", "600mg", 22.00f, "Rx" },
    { "Bedaquiline (Sirturo)", "Antibiotic", "MDR tuberculosis, ATP synthase inhibitor", "400mg", 1800.00f, "Rx" },
    { "Pyrazinamide", "Antibiotic", "Tuberculosis first-line treatment", "500mg", 8.00f, "Rx" },
    { "Ethambutol (Myambutol)", "Antibiotic", "Tuberculosis first-line treatment", "400mg", 12.00f, "Rx" },
    { "Finerenone (Kerendia)", "Heart", "CKD in type 2 diabetes, nonsteroidal MRA", "10mg", 220.00f, "Rx" },
    { "Patiromer (Veltassa)", "Digestive", "Hyperkalemia, potassium binder", "8.4g", 280.00f, "Rx" },
    { "Lanthanum Carbonate (Fosrenol)", "Digestive", "Hyperphosphatemia in ESRD, phosphate binder", "500mg", 180.00f, "Rx" },
    { "Sevelamer (Renagel)", "Digestive", "Hyperphosphatemia in dialysis patients", "800mg", 95.00f, "Rx" },
    { "Darbepoetin Alfa (Aranesp)", "Blood", "Anemia of CKD, chemotherapy-induced anemia", "25mcg/wk", 380.00f, "Rx" },
    { "Dexlansoprazole (Dexilant)", "Digestive", "GERD, erosive esophagitis, dual-release PPI", "30mg", 22.00f, "Rx" },
    { "Prucalopride (Motegrity)", "Digestive", "Chronic idiopathic constipation, 5-HT4 agonist", "2mg", 95.00f, "Rx" },
    { "Methylnaltrexone (Relistor)", "Digestive", "Opioid-induced constipation, peripheral mu antagonist", "8mg", 380.00f, "Rx" },
    { "Budesonide (Entocort)", "Digestive", "Crohn's disease, microscopic colitis, oral steroid", "3mg", 95.00f, "Rx" },
    { "Ursodiol (UDCA)", "Digestive", "Primary biliary cholangitis, gallstones, bile acid", "300mg", 45.00f, "Rx" },
    { "Colesevelam (Welchol)", "Blood", "High LDL, type 2 diabetes, bile acid sequestrant", "625mg", 65.00f, "Rx" },
    { "Granisetron (Kytril)", "Digestive", "Chemotherapy/radiation nausea, 5-HT3 antagonist", "1mg", 28.00f, "Rx" },
    { "Palonosetron (Aloxi)", "Digestive", "Chemotherapy nausea, long-acting 5-HT3 antagonist", "0.25mg IV", 180.00f, "Rx" },
    { "Aprepitant (Emend)", "Digestive", "Chemotherapy nausea, NK1 receptor antagonist", "125mg", 95.00f, "Rx" },
    { "Pancrelipase (Creon)", "Digestive", "Exocrine pancreatic insufficiency, enzyme replacement", "6000u", 280.00f, "Rx" },
    { "Bimatoprost (Lumigan)", "Eye/ENT", "Glaucoma, eyelash growth, prostaglandin analog", "0.01%", 28.00f, "Rx" },
    { "Travoprost (Travatan)", "Eye/ENT", "Glaucoma, IOP lowering, prostaglandin FP agonist", "0.004%", 25.00f, "Rx" },
    { "Netarsudil (Rhopressa)", "Eye/ENT", "Glaucoma, ROCK/NET inhibitor drops", "0.02%", 95.00f, "Rx" },
    { "Faricimab (Vabysmo)", "Eye/ENT", "AMD and DME, bispecific anti-VEGF/Ang-2", "6mg", 2200.00f, "Rx" },
    { "Olopatadine (Pataday)", "Eye/ENT", "Allergic conjunctivitis, H1 antihistamine drops", "0.1%", 12.00f, "Rx" },
    { "Moxifloxacin (Vigamox)", "Eye/ENT", "Bacterial conjunctivitis, fluoroquinolone drops", "0.5%", 18.00f, "Rx" },
    { "Fluorometholone (FML)", "Eye/ENT", "Ocular inflammation, corticosteroid drops", "0.1%", 15.00f, "Rx" },
    { "Calcipotriol (Dovonex)", "Skin", "Psoriasis, vitamin D analog topical", "0.005%", 38.00f, "Rx" },
    { "Clascoterone (Winlevi)", "Skin", "Acne, androgen receptor inhibitor topical", "1%", 85.00f, "Rx" },
    { "Sarecycline (Seysara)", "Skin", "Acne, narrow-spectrum tetracycline antibiotic", "60mg", 95.00f, "Rx" },
    { "Deucravacitinib (Sotyktu)", "Skin", "Plaque psoriasis, TYK2 inhibitor oral", "6mg", 1800.00f, "Rx" },
    { "Spesolimab (Spevigo)", "Skin", "Generalized pustular psoriasis, IL-36R inhibitor", "900mg IV", 18000.00f, "Rx" },
    { "Tapinarof (Vtama)", "Skin", "Plaque psoriasis, AhR agonist topical", "1%", 280.00f, "Rx" },
    { "Tralokinumab (Adbry)", "Skin", "Atopic dermatitis, IL-13 antagonist", "300mg q2wk", 2800.00f, "Rx" },
    { "Lebrikizumab (Ebglyss)", "Skin", "Moderate-severe atopic dermatitis, IL-13 monoclonal", "250mg q2wk", 3000.00f, "Rx" },
    { "Bromocriptine (Cycloset)", "Hormonal", "Hyperprolactinemia, Parkinson's, type 2 diabetes", "2.5mg", 28.00f, "Rx" },
    { "Cinacalcet (Sensipar)", "Hormonal", "Secondary hyperparathyroidism, calcimimetic", "30mg", 180.00f, "Rx" },
    { "Omega-3 FA (Lovaza)", "Blood", "Hypertriglyceridemia, cardiovascular support", "4g/day", 85.00f, "Rx" },
    { "N-Acetylcysteine (NAC)", "Respiratory", "Acetaminophen overdose, COPD mucus, antioxidant", "600mg", 12.99f, "OTC" },
    { "Iron Polysaccharide (Niferex)", "Blood", "Iron deficiency anemia, well-tolerated oral iron", "150mg", 12.99f, "OTC" },
    { "Capsaicin Patch (Qutenza)", "Pain Relief", "Neuropathic pain, peripheral TRPV1 desensitizer", "8%", 650.00f, "Rx" },
    { "Dextromethorphan/Quinidine (Nuedexta)", "Neurological", "Pseudobulbar affect, involuntary laughing/crying", "20/10mg", 280.00f, "Rx" },
    { "Naloxegol (Movantik)", "Digestive", "Opioid-induced constipation, PAMORA", "12.5mg", 180.00f, "Rx" },
    { "Ozanimod (Zeposia)", "Immune", "UC, MS, S1P receptor modulator", "0.92mg", 2800.00f, "Rx" },
    { "Colchicine (Lodoco)", "Heart", "Atherosclerosis prevention, anti-inflammatory", "0.5mg", 48.00f, "Rx" },
    { "Cyclosporine (Sandimmune)", "Immune", "Transplant, autoimmune diseases, calcineurin inhibitor", "25mg", 38.00f, "Rx" },
    { "Leflunomide (Arava)", "Immune", "Rheumatoid arthritis, pyrimidine synthesis inhibitor", "10mg", 28.00f, "Rx" },
    { "Canakinumab (Ilaris)", "Immune", "CAPS, gout flares, IL-1 beta monoclonal antibody", "150mg q8wk", 18000.00f, "Rx" },
    { "Anakinra (Kineret)", "Immune", "RA, CAPS, neonatal onset MAS, IL-1 receptor antagonist", "100mg/day", 850.00f, "Rx" },
    { "Fluticasone/Vilanterol (Breo)", "Respiratory", "Asthma, COPD, once-daily ICS/LABA inhaler", "100/25mcg", 280.00f, "Rx" },
    { "Aclidinium (Tudorza)", "Respiratory", "COPD, twice-daily LAMA bronchodilator", "400mcg", 160.00f, "Rx" },
    { "Hydrocortisone (Solu-Cortef)", "Immune", "Adrenal crisis, severe inflammation, IV steroid", "100mg IV", 12.00f, "Rx" },
    { "Betamethasone (Celestone)", "Immune", "Fetal lung maturity, anti-inflammatory, joint injection", "6mg", 15.00f, "Rx" },
    { "Fluocinolone (Synalar)", "Skin", "Eczema, psoriasis, mid-potency topical steroid", "0.025%", 18.00f, "Rx" },
    { "Phentermine/Topiramate (Qsymia)", "Hormonal", "Obesity, weight management combination", "3.75/23mg", 95.00f, "Rx" },
    { "Naltrexone/Bupropion (Contrave)", "Hormonal", "Obesity, weight management, CNS combination", "8/90mg", 85.00f, "Rx" },
    { "Orlistat (Xenical)", "Digestive", "Obesity, lipase inhibitor, reduces fat absorption", "120mg", 45.00f, "Rx" },
    { "Brompheniramine (Dimetapp)", "Allergy", "Allergies, hay fever, first-gen antihistamine", "4mg", 7.99f, "OTC" },
    { "Carbinoxamine (Palgic)", "Allergy", "Allergic rhinitis, urticaria, antihistamine", "4mg", 12.00f, "Rx" },
    { "Terfenadine (historical)", "Allergy", "Allergic rhinitis, withdrawn due to cardiac risk", "60mg", 0.00f, "Rx" },
    { "Benzocaine (Orajel)", "Pain Relief", "Topical anesthesia, teething, sore throat", "20%", 7.99f, "OTC" },
    { "Bupivacaine (Marcaine)", "Pain Relief", "Regional anesthesia, nerve blocks, local anesthetic", "0.25%", 12.00f, "Rx" },
    { "Ropivacaine (Naropin)", "Pain Relief", "Regional anesthesia, epidural, local anesthetic", "0.2%", 18.00f, "Rx" },
    { "Succinylcholine (Anectine)", "Emergency", "Rapid sequence intubation, depolarizing neuromuscular blocker", "1mg/kg", 22.00f, "Rx" },
    { "Rocuronium (Zemuron)", "Emergency", "Neuromuscular blockade, intubation, non-depolarizing", "0.6mg/kg", 28.00f, "Rx" },
    { "Propofol (Diprivan)", "Emergency", "IV anesthesia induction, ICU sedation", "10mg/mL", 35.00f, "Rx" },
    { "Dexmedetomidine (Precedex)", "Emergency", "ICU sedation, alpha-2 agonist, cooperative sedation", "4mcg/mL", 95.00f, "Rx" },
    { "Midazolam (Versed)", "Emergency", "Procedural sedation, pre-anesthetic, IV benzodiazepine", "1mg", 8.00f, "Rx" },
    { "Phenobarbital (Luminal)", "Neurological", "Epilepsy, status epilepticus, barbiturate", "30mg", 12.00f, "Rx" },
    { "Cenobamate (Xcopri)", "Neurological", "Focal seizures, dual mechanism anticonvulsant", "12.5mg", 750.00f, "Rx" },
    { "Nadolol (Corgard)", "Heart", "Hypertension, angina, non-selective beta-blocker", "20mg", 12.00f, "Rx" },
    { "Nebivolol (Bystolic)", "Heart", "Hypertension, vasodilating beta-1 selective blocker", "5mg", 35.00f, "Rx" },
    { "Nicardipine (Cardene)", "Heart", "Hypertension emergency, CCB IV infusion", "20mg", 35.00f, "Rx" },
    { "Azilsartan (Edarbi)", "Heart", "Hypertension, ARB, highest BP-lowering ARB", "40mg", 45.00f, "Rx" },
    { "Sacubitril (Entresto component)", "Heart", "Combined with valsartan for heart failure", "24mg", 480.00f, "Rx" },
    { "Minoxidil (Rogaine)", "Heart", "Severe hypertension, hair loss, vasodilator", "5mg", 18.00f, "OTC" },
    { "Alpha-Methyldopa (Aldomet)", "Heart", "Hypertension in pregnancy, central alpha-2 agonist", "250mg", 12.00f, "Rx" },
    { "Clonidine (Catapres)", "Heart", "Hypertension, ADHD, alpha-2 agonist, opioid withdrawal", "0.1mg", 8.00f, "Rx" },
    { "Terazosin (Hytrin)", "Heart", "Hypertension, BPH, alpha-1 selective blocker", "1mg", 10.00f, "Rx" },
    { "Prazosin (Minipress)", "Heart", "Hypertension, PTSD nightmares, alpha-1 blocker", "1mg", 8.00f, "Rx" },
    { "Phenoxybenzamine (Dibenzyline)", "Heart", "Pheochromocytoma, irreversible alpha-blocker", "10mg", 1800.00f, "Rx" },
    { "Phentolamine (Oraverse)", "Heart", "Pheochromocytoma crisis, local anesthesia reversal", "5mg", 35.00f, "Rx" },
    { "Allopurinol (Zyloprim)", "Pain Relief", "Gout prevention, xanthine oxidase inhibitor", "100mg", 8.00f, "Rx" },
    { "Rasburicase (Elitek)", "Oncology", "Tumor lysis syndrome prevention, urate oxidase", "0.2mg/kg", 2800.00f, "Rx" },
    { "Lesinurad (Zurampic)", "Pain Relief", "Gout, URAT1 inhibitor, uricosuric", "200mg", 250.00f, "Rx" },
    { "Drospirenone/Ethinyl Estradiol (Yasmin)", "Women's Health", "Combined oral contraceptive, anti-androgenic", "3/0.03mg", 38.00f, "Rx" },
    { "Medroxyprogesterone (Depo-Provera)", "Women's Health", "Contraception, endometriosis, injectable progestin", "150mg/3mo", 45.00f, "Rx" },
    { "Prasterone (Intrarosa)", "Women's Health", "Vaginal atrophy, intravaginal DHEA", "6.5mg", 120.00f, "Rx" },
    { "Fezolinetant (Veozah)", "Women's Health", "Menopausal hot flashes, neurokinin B receptor antagonist", "45mg", 180.00f, "Rx" },
    { "Alprostadil (Muse)", "Men's Health", "Erectile dysfunction, urethral or injection prostaglandin", "500mcg", 95.00f, "Rx" },
    { "Degarelix (Firmagon)", "Men's Health", "Prostate cancer, GnRH antagonist, ADT", "240mg", 850.00f, "Rx" },
    { "Bicalutamide (Casodex)", "Men's Health", "Prostate cancer, androgen receptor antagonist", "50mg", 35.00f, "Rx" },
    { "Potassium Chloride (Klor-Con)", "Vitamin", "Hypokalemia, potassium replacement", "20mEq", 8.99f, "Rx" },
    { "Phosphorus Supplement", "Vitamin", "Hypophosphatemia, bone and energy metabolism", "250mg", 9.99f, "OTC" },
    { "Pyridoxine (Vitamin B6)", "Vitamin", "B6 deficiency, morning sickness, isoniazid neuropathy", "50mg", 6.99f, "OTC" },
    { "Iron Sucrose (Venofer)", "Blood", "Iron deficiency anemia in CKD, IV iron complex", "200mg IV", 280.00f, "Rx" },
    { "Phytonadione (Vitamin K1)", "Blood", "Coagulation factor deficiency, warfarin reversal", "10mg", 12.00f, "Rx" },
    { "Sugammadex (Bridion)", "Emergency", "Rocuronium/vecuronium reversal, cyclodextrin", "200mg IV", 85.00f, "Rx" },
    { "Vasopressin (DDAVP nasal)", "Emergency", "Central DI, enuresis, intranasal formulation", "10mcg", 28.00f, "Rx" },
    { "Dopamine", "Emergency", "Cardiogenic shock, renal perfusion, catecholamine", "200mg IV", 18.00f, "Rx" },
    { "Phenylephrine (Neo-Synephrine)", "Emergency", "Septic shock, nasal congestion, alpha-1 agonist", "10mg IV", 18.00f, "Rx" },
    { "Milrinone (Primacor)", "Emergency", "Acute heart failure, phosphodiesterase 3 inhibitor", "10mg IV", 28.00f, "Rx" },
    { "Levosimendan (Simdax)", "Emergency", "Acute heart failure, calcium sensitizer", "12.5mg", 380.00f, "Rx" },
    { "Protamine (Reversal)", "Emergency", "Heparin reversal agent in cardiac surgery", "50mg IV", 30.00f, "Rx" },
    { "Aminocaproic Acid (Amicar)", "Blood", "Excessive bleeding, antifibrinolytic agent", "1g IV", 22.00f, "Rx" },
    { "Factor VIIa (NovoSeven)", "Blood", "Hemophilia, uncontrolled bleeding, recombinant", "1mg", 3800.00f, "Rx" },
    { "Avatrombopag (Doptelet)", "Blood", "ITP, chronic liver disease thrombocytopenia, TPO-RA", "20mg", 1800.00f, "Rx" },
    { "Luspatercept (Reblozyl)", "Blood", "MDS, beta-thalassemia, erythroid maturation agent", "1mg/kg", 3200.00f, "Rx" },
    { "Enalaprilat (Vasotec IV)", "Heart", "Hypertensive emergency, IV ACE inhibitor", "1.25mg IV", 12.00f, "Rx" },
    { "Clevidipine (Cleviprex)", "Heart", "Hypertensive emergency, ultra-short DHP CCB IV", "0.5mg/mL", 85.00f, "Rx" },
    { "Labetalol (Normodyne)", "Heart", "Hypertensive emergency, alpha/beta blocker IV", "100mg", 15.00f, "Rx" },
    { "Sodium Nitroprusside (Nipride)", "Heart", "Hypertensive emergency, arterial/venous dilator IV", "50mg", 25.00f, "Rx" },
    { "Fenoldopam (Corlopam)", "Heart", "Hypertensive emergency, dopamine D1 agonist IV", "10mg/mL", 45.00f, "Rx" },
    { "Esmolol (Brevibloc)", "Heart", "Perioperative hypertension, SVT, ultra-short beta-1 blocker", "100mg IV", 35.00f, "Rx" },
    { "Scopolamine (Transderm Scop)", "Eye/ENT", "Motion sickness prevention, transdermal patch", "1mg/72hr", 18.00f, "Rx" },
    { "Meclizine (Antivert)", "Eye/ENT", "Vertigo, motion sickness, H1 antihistamine", "25mg", 8.99f, "OTC" },
    { "Dimenhydrinate (Dramamine)", "Eye/ENT", "Motion sickness, nausea, H1 antihistamine", "50mg", 7.49f, "OTC" },
    { "Betahistine (Serc)", "Eye/ENT", "Meniere's disease, vestibular disorder, histamine analog", "16mg", 22.00f, "Rx" },
    { "Desmopressin Nasal (DDAVP)", "Hormonal", "Nocturnal enuresis, diabetes insipidus, intranasal ADH", "10mcg", 95.00f, "Rx" },
    { "Vasopressin (Pitressin)", "Emergency", "Diabetes insipidus, septic shock adjunct", "20units IV", 18.00f, "Rx" },
    { "Octreotide (Somatostatin analog)", "Digestive", "Variceal bleeding, carcinoid syndrome, GI hormone inhibitor", "100mcg", 95.00f, "Rx" },
    { "Terlipressin (Vapreotide)", "Emergency", "Hepatorenal syndrome, variceal bleeding, vasopressin analog", "2mg IV", 280.00f, "Rx" },
    { "Fosphenytoin (Cerebyx)", "Neurological", "Status epilepticus, IV/IM phenytoin prodrug", "150mg PE/mL", 28.00f, "Rx" },
    { "Levetiracetam IV (Keppra IV)", "Neurological", "Status epilepticus, bridge to oral therapy", "500mg IV", 45.00f, "Rx" },
    { "Valproate IV (Depacon)", "Neurological", "Status epilepticus, seizure control IV", "100mg/mL", 35.00f, "Rx" },
    { "Clonazepate (Tranxene)", "Neurological", "Seizures, anxiety, benzodiazepine", "3.75mg", 18.00f, "Rx" },
    { "Clorazepate (Tranxene-SD)", "Mental Health", "Anxiety, seizures, long-acting benzodiazepine", "7.5mg", 18.00f, "Rx" },
    { "Chlordiazepoxide (Librium)", "Mental Health", "Alcohol withdrawal, anxiety, benzodiazepine", "5mg", 12.00f, "Rx" },
    { "Diazepam (Valium IV)", "Emergency", "Status epilepticus, alcohol withdrawal IV benzodiazepine", "10mg IV", 8.00f, "Rx" },
    { "Lorazepam (Ativan IV)", "Emergency", "Status epilepticus, acute agitation, IV benzodiazepine", "2mg IV", 8.00f, "Rx" },
    { "Pantoprazole IV (Protonix IV)", "Digestive", "Active GI bleeding, stress ulcer prophylaxis IV PPI", "40mg IV", 18.00f, "Rx" },
    { "Famotidine IV (Pepcid IV)", "Digestive", "GI bleeding, stress ulcer prophylaxis IV H2 blocker", "20mg IV", 12.00f, "Rx" },
    { "Glycopyrrolate (Robinul)", "Respiratory", "COPD, LAMA bronchodilator, anticholinergic", "0.2mg", 15.00f, "Rx" },
    { "Revefenacin (Yupelri)", "Respiratory", "COPD, once-daily nebulized LAMA", "175mcg", 180.00f, "Rx" },
    { "Olodaterol (Striverdi)", "Respiratory", "COPD maintenance, once-daily LABA", "2.5mcg", 180.00f, "Rx" },
    { "Albuterol/Ipratropium (Combivent)", "Respiratory", "COPD, combined SABA/SAMA bronchodilator", "103/18mcg", 95.00f, "Rx" },
    { "Selpercatinib (Retevmo)", "Oncology", "RET fusion-positive NSCLC and thyroid cancer", "120mg", 2100.00f, "Rx" },
    { "Pralsetinib (Gavreto)", "Oncology", "RET-mutated NSCLC and thyroid cancer", "400mg", 1950.00f, "Rx" },
    { "Brigatinib (Alunbrig)", "Oncology", "ALK-positive NSCLC, second-gen ALK inhibitor", "90mg", 1800.00f, "Rx" },
    { "Ceritinib (Zykadia)", "Oncology", "ALK-positive NSCLC after crizotinib", "450mg", 1700.00f, "Rx" },
    { "Tepotinib (Tepmetko)", "Oncology", "MET exon 14 skipping NSCLC", "500mg", 1900.00f, "Rx" },
    { "Capmatinib (Tabrecta)", "Oncology", "MET exon 14 skipping NSCLC", "400mg", 1850.00f, "Rx" },
    { "Cobimetinib (Cotellic)", "Oncology", "BRAF V600+ melanoma with vemurafenib", "60mg", 1400.00f, "Rx" },
    { "Binimetinib (Mektovi)", "Oncology", "BRAF V600 melanoma MEK inhibitor", "45mg", 1300.00f, "Rx" },
    { "Selumetinib (Koselugo)", "Oncology", "NF1 plexiform neurofibromas, MEK inhibitor", "25mg", 1600.00f, "Rx" },
    { "Encorafenib (Braftovi)", "Oncology", "BRAF V600 melanoma and CRC BRAF inhibitor", "75mg", 1500.00f, "Rx" },
    { "Alpelisib (Piqray)", "Oncology", "PIK3CA-mutated HR+ HER2- breast cancer", "300mg", 1750.00f, "Rx" },
    { "Idelalisib (Zydelig)", "Oncology", "CLL, SLL, follicular lymphoma, PI3Kdelta inhibitor", "150mg", 1600.00f, "Rx" },
    { "Copanlisib (Aliqopa)", "Oncology", "Relapsed follicular lymphoma, pan-PI3K inhibitor", "60mg IV", 1800.00f, "Rx" },
    { "Duvelisib (Copiktra)", "Oncology", "CLL/SLL and follicular lymphoma, PI3Kd/g inhibitor", "25mg", 1500.00f, "Rx" },
    { "Ivosidenib (Tibsovo)", "Oncology", "IDH1-mutated AML and cholangiocarcinoma", "500mg", 2200.00f, "Rx" },
    { "Enasidenib (Idhifa)", "Oncology", "IDH2-mutated relapsed/refractory AML", "100mg", 2100.00f, "Rx" },
    { "Olutasidenib (Rezlidhia)", "Oncology", "IDH1-mutated relapsed/refractory AML", "150mg", 2000.00f, "Rx" },
    { "Pirtobrutinib (Jaypirca)", "Oncology", "CLL/SLL and MCL, non-covalent BTK inhibitor", "200mg", 1900.00f, "Rx" },
    { "Talazoparib (Talzenna)", "Oncology", "BRCA-mutated HER2- breast cancer PARP inhibitor", "1mg", 1400.00f, "Rx" },
    { "Rucaparib (Rubraca)", "Oncology", "BRCA-mutated ovarian cancer PARP inhibitor", "600mg", 1600.00f, "Rx" },
    { "Larotrectinib (Vitrakvi)", "Oncology", "NTRK fusion-positive solid tumors, TRK inhibitor", "100mg", 2100.00f, "Rx" },
    { "Entrectinib (Rozlytrek)", "Oncology", "NTRK/ROS1 fusion-positive solid tumors and NSCLC", "600mg", 1950.00f, "Rx" },
    { "Repotrectinib (Augtyro)", "Oncology", "ROS1-positive NSCLC, next-gen TRK/ROS1 inhibitor", "160mg", 2200.00f, "Rx" },
    { "Lapatinib (Tykerb)", "Oncology", "HER2-positive breast cancer, EGFR/HER2 dual TKI", "1250mg", 1200.00f, "Rx" },
    { "Neratinib (Nerlynx)", "Oncology", "HER2+ breast cancer extended adjuvant therapy", "240mg", 1500.00f, "Rx" },
    { "Tucatinib (Tukysa)", "Oncology", "HER2-positive breast and CRC, HER2-selective TKI", "300mg", 1800.00f, "Rx" },
    { "Asciminib (Scemblix)", "Oncology", "CML BCR-ABL STAMP inhibitor, resistant mutations", "40mg", 1900.00f, "Rx" },
    { "Bosutinib (Bosulif)", "Oncology", "CML BCR-ABL/Src dual kinase inhibitor", "400mg", 1400.00f, "Rx" },
    { "Avapritinib (Ayvakit)", "Oncology", "PDGFRA D842V GIST and systemic mastocytosis", "300mg", 2000.00f, "Rx" },
    { "Ripretinib (Qinlock)", "Oncology", "Advanced GIST, KIT/PDGFRA switch-control inhibitor", "150mg", 1700.00f, "Rx" },
    { "Pexidartinib (Turalio)", "Oncology", "Tenosynovial giant cell tumor, CSF1R inhibitor", "250mg", 1800.00f, "Rx" },
    { "Adagrasib (Krazati)", "Oncology", "KRAS G12C-mutated NSCLC and CRC", "400mg", 1950.00f, "Rx" },
    { "Sotorasib (Lumakras)", "Oncology", "KRAS G12C-mutated NSCLC, first KRAS inhibitor", "960mg", 1900.00f, "Rx" },
    { "Futibatinib (Lytgobi)", "Oncology", "FGFR1-4 altered cholangiocarcinoma", "20mg", 1800.00f, "Rx" },
    { "Erdafitinib (Balversa)", "Oncology", "FGFR-altered urothelial carcinoma", "8mg", 1700.00f, "Rx" },
    { "Infigratinib (Truseltiq)", "Oncology", "FGFR2 fusion cholangiocarcinoma", "125mg", 1800.00f, "Rx" },
    { "Pemigatinib (Pemazyre)", "Oncology", "FGFR1/2/3 altered myeloid/lymphoid and cholangiocarcinoma", "13.5mg", 1900.00f, "Rx" },
    { "Amivantamab (Rybrevant)", "Oncology", "EGFR exon 20 insertion NSCLC, EGFR/MET bispecific", "350mg IV", 2100.00f, "Rx" },
    { "Panitumumab (Vectibix)", "Oncology", "RAS wild-type colorectal cancer, anti-EGFR mAb", "6mg/kg IV", 1200.00f, "Rx" },
    { "Necitumumab (Portrazza)", "Oncology", "Squamous NSCLC with gemcitabine/cisplatin", "800mg IV", 1400.00f, "Rx" },
    { "Ramucirumab (Cyramza)", "Oncology", "Gastric, NSCLC, CRC, HCC VEGFR2 antibody", "8mg/kg IV", 1500.00f, "Rx" },
    { "Axitinib (Inlyta)", "Oncology", "Advanced RCC second-line VEGFR TKI", "5mg", 1100.00f, "Rx" },
    { "Vandetanib (Caprelsa)", "Oncology", "Medullary thyroid cancer, VEGFR/RET/EGFR TKI", "300mg", 1300.00f, "Rx" },
    { "Regorafenib (Stivarga)", "Oncology", "Colorectal and GIST and HCC multikinase inhibitor", "160mg", 1200.00f, "Rx" },
    { "Ado-Trastuzumab Emtansine (Kadcyla)", "Oncology", "HER2+ breast cancer ADC with DM1 cytotoxin", "3.6mg/kg IV", 2400.00f, "Rx" },
    { "Trastuzumab Deruxtecan (Enhertu)", "Oncology", "HER2+ breast/gastric/NSCLC, ADC with DXd topoisomerase payload", "5.4mg/kg IV", 2800.00f, "Rx" },
    { "Sacituzumab Govitecan (Trodelvy)", "Oncology", "Triple-negative breast and urothelial cancer ADC", "10mg/kg IV", 2500.00f, "Rx" },
    { "Enfortumab Vedotin (Padcev)", "Oncology", "Urothelial cancer nectin-4 ADC with MMAE", "1.25mg/kg IV", 2600.00f, "Rx" },
    { "Polatuzumab Vedotin (Polivy)", "Oncology", "Diffuse large B-cell lymphoma CD79b ADC", "1.8mg/kg IV", 2400.00f, "Rx" },
    { "Loncastuximab Tesirine (Zynlonta)", "Oncology", "Relapsed DLBCL CD19 ADC with pyrrolobenzodiazepine", "0.15mg/kg IV", 2200.00f, "Rx" },
    { "Mirvetuximab Soravtansine (Elahere)", "Oncology", "FRalpha-high ovarian cancer ADC with DM4", "6mg/kg IV", 2300.00f, "Rx" },
    { "Tisotumab Vedotin (Tivdak)", "Oncology", "Cervical cancer tissue factor ADC with MMAE", "2mg/kg IV", 2100.00f, "Rx" },
    { "Brentuximab Vedotin (Adcetris)", "Oncology", "Hodgkin lymphoma and ALCL CD30 ADC", "1.8mg/kg IV", 2200.00f, "Rx" },
    { "Gemtuzumab Ozogamicin (Mylotarg)", "Oncology", "CD33+ AML antibody-drug conjugate with calicheamicin", "3mg/m2 IV", 1900.00f, "Rx" },
    { "Inotuzumab Ozogamicin (Besylomab)", "Oncology", "Relapsed B-cell ALL CD22 ADC", "0.8mg/m2 IV", 2000.00f, "Rx" },
    { "Dostarlimab (Jemperli)", "Oncology", "dMMR endometrial cancer and solid tumors, anti-PD-1", "500mg IV", 2000.00f, "Rx" },
    { "Cemiplimab (Libtayo)", "Oncology", "Cutaneous SCC, BCC, NSCLC, anti-PD-1 antibody", "350mg IV", 1900.00f, "Rx" },
    { "Avelumab (Bavencio)", "Oncology", "Merkel cell carcinoma and urothelial, anti-PD-L1", "800mg IV", 1800.00f, "Rx" },
    { "Tremelimumab (Imjudo)", "Oncology", "HCC and NSCLC with durvalumab, anti-CTLA-4", "75mg IV", 1700.00f, "Rx" },
    { "Isatuximab (Sarclisa)", "Oncology", "Relapsed multiple myeloma CD38 antibody", "10mg/kg IV", 2100.00f, "Rx" },
    { "Elotuzumab (Empliciti)", "Oncology", "Relapsed multiple myeloma SLAMF7 antibody", "10mg/kg IV", 1900.00f, "Rx" },
    { "Selinexor (Xpovio)", "Oncology", "Multiple myeloma and DLBCL, XPO1 inhibitor", "80mg", 1800.00f, "Rx" },
    { "Belantamab Mafodotin (Blenrep)", "Oncology", "BCMA-directed ADC for multiple myeloma", "2.5mg/kg IV", 2200.00f, "Rx" },
    { "Daratumumab (Darzalex)", "Oncology", "Multiple myeloma CD38 monoclonal antibody", "16mg/kg IV", 2400.00f, "Rx" },
    { "Blinatumomab (Blincyto)", "Oncology", "B-cell ALL CD19/CD3 bispecific T-cell engager", "28mcg/day IV", 2800.00f, "Rx" },
    { "Mosunetuzumab (Lunsumio)", "Oncology", "Follicular lymphoma CD20/CD3 bispecific antibody", "1mg IV", 2200.00f, "Rx" },
    { "Glofitamab (Columvi)", "Oncology", "DLBCL CD20/CD3 bispecific T-cell engager", "2.5mg IV", 2300.00f, "Rx" },
    { "Epcoritamab (Epkinly)", "Oncology", "DLBCL subcutaneous CD3/CD20 bispecific", "48mg SC", 2200.00f, "Rx" },
    { "Teclistamab (Tecvayli)", "Oncology", "Relapsed multiple myeloma BCMA/CD3 bispecific", "1.5mg/kg SC", 2400.00f, "Rx" },
    { "Elranatamab (Elrexfio)", "Oncology", "Multiple myeloma BCMA/CD3 bispecific antibody", "76mg SC", 2300.00f, "Rx" },
    { "Talquetamab (Talvey)", "Oncology", "Multiple myeloma GPRC5D/CD3 bispecific antibody", "0.4mg/kg SC", 2200.00f, "Rx" },
    { "Axicabtagene Ciloleucel (Yescarta)", "Oncology", "DLBCL and follicular lymphoma CAR-T CD19 therapy", "2x10^6/kg IV", 390000.00f, "Rx" },
    { "Tisagenlecleucel (Kymriah)", "Oncology", "B-cell ALL and DLBCL CD19 CAR-T cell therapy", "3x10^8/kg IV", 475000.00f, "Rx" },
    { "Lisocabtagene Maraleucel (Breyanzi)", "Oncology", "DLBCL and CLL CD19 CAR-T defined ratio", "90x10^6 IV", 410000.00f, "Rx" },
    { "Idecabtagene Vicleucel (Abecma)", "Oncology", "Relapsed multiple myeloma BCMA CAR-T therapy", "450x10^6 IV", 420000.00f, "Rx" },
    { "Ciltacabtagene Autoleucel (Carvykti)", "Oncology", "Multiple myeloma BCMA CAR-T with high response rate", "0.75x10^6/kg IV", 465000.00f, "Rx" },
    { "Margetuximab (Margenza)", "Oncology", "HER2+ breast cancer Fc-optimized anti-HER2 antibody", "15mg/kg IV", 1600.00f, "Rx" },
    { "Zolbetuximab (Vyloy)", "Oncology", "CLDN18.2+ gastric/GEJ adenocarcinoma antibody", "800mg/m2 IV", 1800.00f, "Rx" },
    { "Olaratumab (Lartruvo)", "Oncology", "Soft tissue sarcoma PDGFRA antibody with doxorubicin", "15mg/kg IV", 1400.00f, "Rx" },
    { "Zanubrutinib (Brukinsa)", "Oncology", "CLL, MCL, WM, BTK inhibitor second-gen", "160mg", 1800.00f, "Rx" },
    { "Abatacept (Orencia)", "Immune", "Rheumatoid arthritis, CTLA-4 Ig costimulation blocker", "500mg IV", 1400.00f, "Rx" },
    { "Baricitinib (Olumiant)", "Immune", "RA, alopecia areata, COVID-19, JAK1/2 inhibitor", "4mg", 1200.00f, "Rx" },
    { "Upadacitinib (Rinvoq)", "Immune", "RA, PsA, AS, UC, AD, JAK1 selective inhibitor", "15mg", 1300.00f, "Rx" },
    { "Filgotinib (Jyseleca)", "Immune", "RA, UC, JAK1 preferential inhibitor", "200mg", 1200.00f, "Rx" },
    { "Deucravacitinib (Sotyktu)", "Skin", "Plaque psoriasis, TYK2 inhibitor, oral", "6mg", 1100.00f, "Rx" },
    { "Bimekizumab (Bimzelx)", "Skin", "Plaque psoriasis, IL-17A and IL-17F inhibitor", "320mg SC", 1600.00f, "Rx" },
    { "Spesolimab (Spevigo)", "Skin", "Generalized pustular psoriasis IL-36R antagonist", "900mg IV", 2200.00f, "Rx" },
    { "Mirikizumab (Omvoh)", "Immune", "Ulcerative colitis IL-23p19 inhibitor", "300mg IV", 1400.00f, "Rx" },
    { "Risankizumab (Skyrizi)", "Skin", "Plaque psoriasis, PsA, CD, UC IL-23 inhibitor", "150mg SC", 1500.00f, "Rx" },
    { "Guselkumab (Tremfya)", "Skin", "Plaque psoriasis and PsA, selective IL-23 inhibitor", "100mg SC", 1400.00f, "Rx" },
    { "Tildrakizumab (Ilumya)", "Skin", "Plaque psoriasis, anti-IL-23p19 antibody", "100mg SC", 1300.00f, "Rx" },
    { "Ixekizumab (Taltz)", "Skin", "Psoriasis, PsA, AS, IL-17A selective inhibitor", "80mg SC", 1400.00f, "Rx" },
    { "Brolucizumab (Beovu)", "Eye/ENT", "Wet AMD and DME, anti-VEGF single-chain antibody", "6mg intraocular", 1800.00f, "Rx" },
    { "Faricimab (Vabysmo)", "Eye/ENT", "AMD and DME, bispecific anti-VEGF/Ang-2 antibody", "6mg intraocular", 1900.00f, "Rx" },
    { "Pegcetacoplan (Syfovre)", "Eye/ENT", "Geographic atrophy (dry AMD) complement C3 inhibitor", "15mg intraocular", 2200.00f, "Rx" },
    { "Avacincaptad Pegol (Izervay)", "Eye/ENT", "Geographic atrophy complement C5 inhibitor", "2mg intraocular", 2100.00f, "Rx" },
    { "Omidenepag Isopropyl (Eybelis)", "Eye/ENT", "Glaucoma and ocular hypertension, EP2 receptor agonist", "0.002%", 45.00f, "Rx" },
    { "Netarsudil (Rhopressa)", "Eye/ENT", "Glaucoma, ROCK/NET inhibitor, rho kinase", "0.02%", 58.00f, "Rx" },
    { "Latanoprostene Bunod (Vyzulta)", "Eye/ENT", "Glaucoma, NO-donating prostaglandin analog", "0.024%", 52.00f, "Rx" },
    { "Loteprednol Etabonate (Lotemax)", "Eye/ENT", "Ocular inflammation and post-op, ophthalmic steroid", "0.5%", 38.00f, "Rx" },
    { "Cyclosporine Ophthalmic (Restasis)", "Eye/ENT", "Dry eye disease, immunomodulator ophthalmic", "0.05%", 48.00f, "Rx" },
    { "Lifitegrast (Xiidra)", "Eye/ENT", "Dry eye disease, LFA-1 integrin antagonist", "5%", 55.00f, "Rx" },
    { "Perfluorocarbon (Perfluoron)", "Eye/ENT", "Retinal detachment surgery, vitreoretinal liquid", "various", 85.00f, "Rx" },
    { "Verteporfin (Visudyne)", "Eye/ENT", "Wet AMD photodynamic therapy, photosensitizer", "6mg/m2 IV", 1200.00f, "Rx" },
    { "Ranibizumab (Lucentis)", "Eye/ENT", "Wet AMD and DME anti-VEGF antibody fragment", "0.5mg intraocular", 1900.00f, "Rx" },
    { "Ocrelizumab (Ocrevus)", "Neurological", "MS relapsing and primary progressive, anti-CD20", "300mg IV", 2200.00f, "Rx" },
    { "Ofatumumab (Kesimpta)", "Neurological", "Relapsing MS subcutaneous anti-CD20 antibody", "20mg SC", 1900.00f, "Rx" },
    { "Ublituximab (Briumvi)", "Neurological", "Relapsing MS, anti-CD20 glycoengineered antibody", "450mg IV", 1800.00f, "Rx" },
    { "Ozanimod (Zeposia)", "Neurological", "Relapsing MS and UC, S1P1/5 receptor modulator", "0.92mg", 1500.00f, "Rx" },
    { "Siponimod (Mayzent)", "Neurological", "Secondary progressive MS, S1P1/5 modulator", "2mg", 1400.00f, "Rx" },
    { "Ponesimod (Ponvory)", "Neurological", "Relapsing MS, selective S1P1 modulator", "20mg", 1400.00f, "Rx" },
    { "Cladribine (Mavenclad)", "Neurological", "Active relapsing MS, purine nucleoside analogue", "10mg", 1600.00f, "Rx" },
    { "Diroximel Fumarate (Vumerity)", "Neurological", "Relapsing MS, fumaric acid ester, GI-improved DMF", "462mg", 850.00f, "Rx" },
    { "Monomethyl Fumarate (Bafiertam)", "Neurological", "Relapsing MS, fumaric acid ester active metabolite", "95mg", 800.00f, "Rx" },
    { "Eptinezumab (Vyepti)", "Neurological", "Migraine prevention, IV anti-CGRP antibody", "100mg IV", 750.00f, "Rx" },
    { "Fremanezumab (Ajovy)", "Neurological", "Migraine prevention, subcutaneous anti-CGRP antibody", "225mg SC", 700.00f, "Rx" },
    { "Galcanezumab (Emgality)", "Neurological", "Migraine and cluster headache prevention, anti-CGRP", "120mg SC", 720.00f, "Rx" },
    { "Rimegepant (Nurtec ODT)", "Neurological", "Acute migraine and prevention, CGRP receptor antagonist", "75mg", 85.00f, "Rx" },
    { "Atogepant (Qulipta)", "Neurological", "Episodic migraine prevention, oral CGRP antagonist", "60mg", 95.00f, "Rx" },
    { "Zavegepant (Zavzpret)", "Neurological", "Acute migraine, intranasal CGRP receptor antagonist", "10mg nasal", 90.00f, "Rx" },
    { "Lasmiditan (Reyvow)", "Neurological", "Acute migraine, 5-HT1F agonist, ditan class", "100mg", 60.00f, "Rx" },
    { "Incobotulinumtoxin A (Xeomin)", "Neurological", "Cervical dystonia, spasticity, migraine, pure BoNT-A", "50units IM", 480.00f, "Rx" },
    { "Abobotulinumtoxin A (Dysport)", "Neurological", "Cervical dystonia, spasticity, glabellar lines", "500units IM", 480.00f, "Rx" },
    { "Rimabotulinumtoxin B (Myobloc)", "Neurological", "Cervical dystonia, type B botulinum toxin", "5000units IM", 490.00f, "Rx" },
    { "Safinamide (Xadago)", "Neurological", "Parkinson's MAO-B inhibitor and glutamate modulator", "50mg", 380.00f, "Rx" },
    { "Opicapone (Ongentys)", "Neurological", "Parkinson's COMT inhibitor, once daily, adjunct to levodopa", "50mg", 280.00f, "Rx" },
    { "Istradefylline (Nourianz)", "Neurological", "Parkinson's, adenosine A2A receptor antagonist", "20mg", 320.00f, "Rx" },
    { "Valbenazine (Ingrezza)", "Neurological", "Tardive dyskinesia, selective VMAT2 inhibitor", "40mg", 1400.00f, "Rx" },
    { "Deutetrabenazine (Austedo)", "Neurological", "Tardive dyskinesia and chorea in HD, VMAT2 inhibitor", "6mg", 1200.00f, "Rx" },
    { "Inebilizumab (Uplizna)", "Neurological", "NMOSD, anti-CD19 B-cell depleting antibody", "300mg IV", 2000.00f, "Rx" },
    { "Satralizumab (Enspryng)", "Neurological", "NMOSD, anti-IL-6 receptor recycling antibody", "120mg SC", 1800.00f, "Rx" },
    { "Eculizumab (Soliris)", "Immune", "PNH, aHUS, gMG, NMOSD, anti-C5 complement antibody", "900mg IV", 6800.00f, "Rx" },
    { "Ravulizumab (Ultomiris)", "Immune", "PNH, aHUS, gMG, long-acting anti-C5 antibody", "2400mg IV", 7200.00f, "Rx" },
    { "Avacopan (Tavneos)", "Immune", "ANCA vasculitis, oral C5aR1 complement inhibitor", "30mg", 1800.00f, "Rx" },
    { "Nusinersen (Spinraza)", "Neurological", "Spinal muscular atrophy, antisense oligonucleotide, intrathecal", "12mg intrathecal", 125000.00f, "Rx" },
    { "Onasemnogene Abeparvovec (Zolgensma)", "Neurological", "SMA type 1, one-time AAV9 SMN1 gene therapy IV", "1.1x10^14 vg/kg IV", 2100000.00f, "Rx" },
    { "Risdiplam (Evrysdi)", "Neurological", "SMA all types, oral SMN2 splicing modifier", "0.2mg/kg", 340000.00f, "Rx" },
    { "Lecanemab (Leqembi)", "Neurological", "Early Alzheimer's, anti-amyloid beta antibody", "10mg/kg IV", 26500.00f, "Rx" },
    { "Donanemab (Kisunla)", "Neurological", "Early Alzheimer's, anti-amyloid beta plaque antibody", "700mg IV", 32000.00f, "Rx" },
    { "Trofinetide (Daybue)", "Neurological", "Rett syndrome, IGF-1 analogue neurotrophic peptide", "200mg/kg", 680000.00f, "Rx" },
    { "Fenfluramine (Fintepla)", "Neurological", "Dravet syndrome seizures, serotonin releaser", "0.1mg/kg", 1800.00f, "Rx" },
    { "Ganaxolone (Ztalmy)", "Neurological", "CDKL5 deficiency disorder seizures, neurosteroid", "300mg", 1600.00f, "Rx" },
    { "Carisbamate (Carbatrol adjunct)", "Neurological", "Focal seizures, novel anticonvulsant", "100mg", 380.00f, "Rx" },
    { "Buprenorphine/Naloxone (Suboxone)", "Mental Health", "Opioid use disorder maintenance, sublingual film", "8/2mg", 85.00f, "Rx" },
    { "Naltrexone XR (Vivitrol)", "Mental Health", "Alcohol and opioid use disorder, monthly IM injection", "380mg IM", 1200.00f, "Rx" },
    { "Acamprosate (Campral)", "Mental Health", "Alcohol use disorder relapse prevention, GABA/NMDA", "666mg", 38.00f, "Rx" },
    { "Disulfiram (Antabuse)", "Mental Health", "Alcohol use disorder aversion therapy, ALDH inhibitor", "250mg", 28.00f, "Rx" },
    { "Varenicline (Chantix)", "Mental Health", "Nicotine addiction, partial nAChR agonist", "1mg", 95.00f, "Rx" },
    { "Bupropion SR (Zyban)", "Mental Health", "Smoking cessation, norepinephrine/dopamine reuptake inhibitor", "150mg", 42.00f, "Rx" },
    { "Lofexidine (Lucemyra)", "Mental Health", "Opioid withdrawal, non-opioid alpha-2 agonist", "0.18mg", 85.00f, "Rx" },
    { "Pitolisant (Wakix)", "Mental Health", "Narcolepsy and EDS, histamine H3 receptor antagonist", "17.8mg", 380.00f, "Rx" },
    { "Solriamfetol (Sunosi)", "Mental Health", "Narcolepsy/OSA EDS, dopamine/norepinephrine reuptake inhibitor", "75mg", 280.00f, "Rx" },
    { "Lemborexant (Dayvigo)", "Mental Health", "Insomnia, dual orexin receptor antagonist", "5mg", 38.00f, "Rx" },
    { "Daridorexant (Quviviq)", "Mental Health", "Insomnia, dual orexin receptor antagonist 1/2", "25mg", 42.00f, "Rx" },
    { "Zuranolone (Zurzuvae)", "Mental Health", "Major depressive disorder, neuroactive steroid GABA-A modulator", "50mg", 750.00f, "Rx" },
    { "Brexanolone (Zulresso)", "Mental Health", "Postpartum depression, IV allopregnanolone GABA-A PAM", "90mcg/kg/hr IV", 35000.00f, "Rx" },
    { "Gepirone ER (Exxua)", "Mental Health", "Major depressive disorder, serotonin 5-HT1A agonist", "20mg", 95.00f, "Rx" },
    { "Vilazodone (Viibryd)", "Mental Health", "Major depression, SSRI plus 5-HT1A partial agonist", "40mg", 88.00f, "Rx" },
    { "Vortioxetine (Trintellix)", "Mental Health", "Major depression, multimodal serotonin modulator", "20mg", 95.00f, "Rx" },
    { "Levomilnacipran (Fetzima)", "Mental Health", "Major depression, SNRI with NE-preferring profile", "80mg", 85.00f, "Rx" },
    { "Phenelzine (Nardil)", "Mental Health", "Atypical depression, irreversible MAO-A/B inhibitor", "15mg", 32.00f, "Rx" },
    { "Tranylcypromine (Parnate)", "Mental Health", "Major depression, irreversible non-selective MAOI", "10mg", 28.00f, "Rx" },
    { "Isocarboxazid (Marplan)", "Mental Health", "Treatment-resistant depression, irreversible MAOI", "10mg", 35.00f, "Rx" },
    { "Selegiline Patch (Emsam)", "Mental Health", "Major depression, transdermal selective MAO-B inhibitor", "6mg/24hr", 185.00f, "Rx" },
    { "Amitriptyline (Elavil)", "Mental Health", "Depression, neuropathic pain, TCA with anticholinergic", "25mg", 12.00f, "Rx" },
    { "Nortriptyline (Pamelor)", "Mental Health", "Depression and neuropathic pain, TCA secondary amine", "25mg", 14.00f, "Rx" },
    { "Imipramine (Tofranil)", "Mental Health", "Depression, enuresis, TCA with strong anticholinergic", "25mg", 12.00f, "Rx" },
    { "Clomipramine (Anafranil)", "Mental Health", "OCD and depression, TCA with serotonergic activity", "25mg", 18.00f, "Rx" },
    { "Doxepin (Sinequan)", "Mental Health", "Depression, anxiety, insomnia, TCA with H1 antihistamine", "10mg", 15.00f, "Rx" },
    { "Desipramine (Norpramin)", "Mental Health", "Depression, TCA with norepinephrine preference", "25mg", 16.00f, "Rx" },
    { "Maprotiline (Ludiomil)", "Mental Health", "Depression, tetracyclic NE reuptake inhibitor", "25mg", 22.00f, "Rx" },
    { "Haloperidol (Haldol)", "Mental Health", "Schizophrenia, delirium, Tourette's, potent D2 blocker", "5mg", 12.00f, "Rx" },
    { "Chlorpromazine (Thorazine)", "Mental Health", "Schizophrenia, first antipsychotic, low-potency D2 blocker", "100mg", 10.00f, "Rx" },
    { "Fluphenazine (Prolixin)", "Mental Health", "Schizophrenia, high-potency phenothiazine antipsychotic", "1mg", 14.00f, "Rx" },
    { "Perphenazine (Trilafon)", "Mental Health", "Schizophrenia and nausea, mid-potency phenothiazine", "4mg", 12.00f, "Rx" },
    { "Trifluoperazine (Stelazine)", "Mental Health", "Schizophrenia and anxiety, piperazine phenothiazine", "2mg", 12.00f, "Rx" },
    { "Thioridazine (Mellaril)", "Mental Health", "Schizophrenia, low-potency with QTc risk, phenothiazine", "25mg", 10.00f, "Rx" },
    { "Thiothixene (Navane)", "Mental Health", "Schizophrenia, thioxanthene typical antipsychotic", "2mg", 14.00f, "Rx" },
    { "Loxapine (Loxitane)", "Mental Health", "Schizophrenia, dibenzoxazepine antipsychotic", "10mg", 18.00f, "Rx" },
    { "Molindone (Moban)", "Mental Health", "Schizophrenia, dihydroindolone typical antipsychotic", "10mg", 20.00f, "Rx" },
    { "Pimozide (Orap)", "Mental Health", "Tourette's syndrome, diphenylbutylpiperidine antipsychotic", "1mg", 28.00f, "Rx" },
    { "Iloperidone (Fanapt)", "Mental Health", "Schizophrenia, atypical antipsychotic D2/5-HT2A", "6mg", 85.00f, "Rx" },
    { "Asenapine (Saphris)", "Mental Health", "Schizophrenia and bipolar, sublingual atypical antipsychotic", "5mg SL", 95.00f, "Rx" },
    { "Lurasidone (Latuda)", "Mental Health", "Schizophrenia and bipolar depression, atypical antipsychotic", "40mg", 95.00f, "Rx" },
    { "Paliperidone (Invega)", "Mental Health", "Schizophrenia, active metabolite of risperidone", "3mg", 85.00f, "Rx" },
    { "Pimavanserin (Nuplazid)", "Mental Health", "Parkinson's disease psychosis, 5-HT2A inverse agonist", "34mg", 1200.00f, "Rx" },
    { "Lumateperone (Caplyta)", "Mental Health", "Schizophrenia and bipolar depression, multimodal atypical", "42mg", 380.00f, "Rx" },
    { "Cariprazine (Vraylar)", "Mental Health", "Schizophrenia, bipolar I, MDD adjunct, D3/D2 partial agonist", "1.5mg", 980.00f, "Rx" },
    { "Brexpiprazole (Rexulti)", "Mental Health", "Schizophrenia and MDD adjunct, D2/5-HT1A partial agonist", "2mg", 850.00f, "Rx" },
    { "Olanzapine/Samidorphan (Lybalvi)", "Mental Health", "Schizophrenia/bipolar, olanzapine plus opioid blocker", "10/10mg", 680.00f, "Rx" },
    { "Lisdexamfetamine (Vyvanse)", "Mental Health", "ADHD and BED, prodrug amphetamine, long-acting", "30mg", 95.00f, "Rx" },
    { "Amphetamine Mixed Salts (Adderall)", "Mental Health", "ADHD and narcolepsy, dopamine/NE releasing agent", "10mg", 38.00f, "Rx" },
    { "Dextroamphetamine (Dexedrine)", "Mental Health", "ADHD and narcolepsy, pure d-amphetamine", "5mg", 32.00f, "Rx" },
    { "Dexmethylphenidate (Focalin)", "Mental Health", "ADHD, active d-threo isomer of methylphenidate", "5mg", 45.00f, "Rx" },
    { "Methylphenidate ER (Concerta)", "Mental Health", "ADHD, osmotic pump extended-release methylphenidate", "18mg", 55.00f, "Rx" },
    { "Viloxazine (Qelbree)", "Mental Health", "ADHD in pediatric patients, selective NE reuptake inhibitor", "100mg", 85.00f, "Rx" },
    { "Guanfacine ER (Intuniv)", "Mental Health", "ADHD, alpha-2A agonist, non-stimulant option", "1mg", 65.00f, "Rx" },
    { "Doxepin (Silenor)", "Mental Health", "Insomnia sleep maintenance, very low dose H1 blocker", "3mg", 55.00f, "Rx" },
    { "Triazolam (Halcion)", "Mental Health", "Insomnia short-term, ultra-short triazolo-benzodiazepine", "0.25mg", 28.00f, "Rx" },
    { "Temazepam (Restoril)", "Mental Health", "Insomnia, intermediate benzodiazepine sleep aid", "15mg", 25.00f, "Rx" },
    { "Flurazepam (Dalmane)", "Mental Health", "Insomnia, long-acting benzodiazepine hypnotic", "15mg", 22.00f, "Rx" },
    { "Oxazepam (Serax)", "Mental Health", "Anxiety and alcohol withdrawal, short-acting benzodiazepine", "15mg", 22.00f, "Rx" },
    { "Estazolam (ProSom)", "Mental Health", "Insomnia, intermediate triazolo-benzodiazepine", "1mg", 24.00f, "Rx" },
    { "Quazepam (Doral)", "Mental Health", "Insomnia, BZ1-selective benzodiazepine hypnotic", "7.5mg", 28.00f, "Rx" },
    { "Meprobamate (Miltown)", "Mental Health", "Anxiety, carbamate anxiolytic, sedative-hypnotic", "400mg", 18.00f, "Rx" },
    { "Emtricitabine (Emtriva)", "Antiviral", "HIV, nucleoside reverse transcriptase inhibitor, NRTI", "200mg", 45.00f, "Rx" },
    { "Abacavir (Ziagen)", "Antiviral", "HIV, carbocyclic nucleoside NRTI with HLA-B5701 screening", "300mg", 55.00f, "Rx" },
    { "Lamivudine (Epivir)", "Antiviral", "HIV and HBV, cytidine analogue NRTI", "300mg", 42.00f, "Rx" },
    { "Zidovudine (Retrovir)", "Antiviral", "HIV, first approved antiretroviral, thymidine analogue NRTI", "300mg", 45.00f, "Rx" },
    { "Stavudine (Zerit)", "Antiviral", "HIV, thymidine nucleoside NRTI, mitochondrial toxicity risk", "30mg", 38.00f, "Rx" },
    { "Didanosine (Videx)", "Antiviral", "HIV, adenosine nucleoside NRTI, pancreatitis risk", "400mg", 42.00f, "Rx" },
    { "Tenofovir Alafenamide (Vemlidy)", "Antiviral", "HIV and HBV, prodrug TAF with less renal/bone effects", "25mg", 68.00f, "Rx" },
    { "Rilpivirine (Edurant)", "Antiviral", "HIV, second-gen non-nucleoside reverse transcriptase inhibitor", "25mg", 85.00f, "Rx" },
    { "Doravirine (Pifeltro)", "Antiviral", "HIV, novel NNRTI with activity against common NNRTI mutations", "100mg", 95.00f, "Rx" },
    { "Efavirenz (Sustiva)", "Antiviral", "HIV, NNRTI with CNS side effects and long half-life", "600mg", 72.00f, "Rx" },
    { "Etravirine (Intelence)", "Antiviral", "HIV, second-gen NNRTI for NNRTI-experienced patients", "200mg", 95.00f, "Rx" },
    { "Nevirapine (Viramune)", "Antiviral", "HIV, first-gen NNRTI with hepatotoxicity risk", "200mg", 48.00f, "Rx" },
    { "Dolutegravir (Tivicay)", "Antiviral", "HIV, second-gen integrase strand transfer inhibitor", "50mg", 110.00f, "Rx" },
    { "Bictegravir (Biktarvy component)", "Antiviral", "HIV, third-gen INSTI in Biktarvy combination", "50mg", 120.00f, "Rx" },
    { "Cabotegravir (Vocabria)", "Antiviral", "HIV treatment and PrEP, INSTI, oral and long-acting IM", "30mg", 145.00f, "Rx" },
    { "Elvitegravir (Vitekta)", "Antiviral", "HIV INSTI, requires pharmacokinetic booster", "150mg", 95.00f, "Rx" },
    { "Raltegravir (Isentress)", "Antiviral", "HIV, first approved INSTI, twice-daily dosing", "400mg", 88.00f, "Rx" },
    { "Darunavir (Prezista)", "Antiviral", "HIV protease inhibitor, requires ritonavir/cobicistat boost", "800mg", 95.00f, "Rx" },
    { "Atazanavir (Reyataz)", "Antiviral", "HIV protease inhibitor, once-daily with ritonavir boost", "300mg", 85.00f, "Rx" },
    { "Lopinavir/Ritonavir (Kaletra)", "Antiviral", "HIV, co-formulated protease inhibitor with ritonavir boost", "200/50mg", 75.00f, "Rx" },
    { "Fosamprenavir (Lexiva)", "Antiviral", "HIV, prodrug of amprenavir protease inhibitor", "700mg", 80.00f, "Rx" },
    { "Tipranavir (Aptivus)", "Antiviral", "HIV, non-peptidic PI for multi-resistant virus", "500mg", 95.00f, "Rx" },
    { "Maraviroc (Selzentry)", "Antiviral", "HIV CCR5 co-receptor antagonist, entry inhibitor", "300mg", 88.00f, "Rx" },
    { "Enfuvirtide (Fuzeon)", "Antiviral", "HIV, fusion inhibitor peptide, subcutaneous injection", "90mg SC", 145.00f, "Rx" },
    { "Ibalizumab (Trogarzo)", "Antiviral", "Multidrug-resistant HIV, CD4 receptor post-attachment inhibitor", "800mg IV", 250.00f, "Rx" },
    { "Fostemsavir (Rukobia)", "Antiviral", "HIV, gp120 attachment inhibitor for highly resistant virus", "600mg", 280.00f, "Rx" },
    { "Lenacapavir (Sunlenca)", "Antiviral", "HIV capsid inhibitor, twice-yearly SC injection for MDR", "927mg SC", 450.00f, "Rx" },
    { "Rilpivirine/Cabotegravir (Cabenuva)", "Antiviral", "HIV complete long-acting monthly IM regimen", "400/600mg IM", 2800.00f, "Rx" },
    { "Bictegravir/TAF/FTC (Biktarvy)", "Antiviral", "HIV complete single-tablet regimen, INSTI+2 NRTIs", "50/25/200mg", 385.00f, "Rx" },
    { "Dolutegravir/Abacavir/Lamivudine (Triumeq)", "Antiviral", "HIV complete single-tablet regimen", "50/600/300mg", 290.00f, "Rx" },
    { "Ganciclovir (Cytovene)", "Antiviral", "CMV retinitis and prevention, nucleoside analogue IV", "5mg/kg IV", 85.00f, "Rx" },
    { "Valganciclovir (Valcyte)", "Antiviral", "CMV prevention and retinitis, prodrug of ganciclovir oral", "900mg", 75.00f, "Rx" },
    { "Foscarnet (Foscavir)", "Antiviral", "Acyclovir-resistant HSV/CMV, pyrophosphate analogue IV", "60mg/kg IV", 125.00f, "Rx" },
    { "Cidofovir (Vistide)", "Antiviral", "CMV retinitis in AIDS, nephrotoxic nucleotide analogue IV", "5mg/kg IV", 145.00f, "Rx" },
    { "Letermovir (Prevymis)", "Antiviral", "CMV prophylaxis post-transplant, viral terminase inhibitor", "480mg", 195.00f, "Rx" },
    { "Maribavir (Livtencity)", "Antiviral", "CMV treatment post-transplant, UL97 kinase inhibitor", "400mg", 210.00f, "Rx" },
    { "Nirmatrelvir/Ritonavir (Paxlovid)", "Antiviral", "COVID-19, Mpro protease inhibitor with PK booster", "300/100mg", 530.00f, "Rx" },
    { "Molnupiravir (Lagevrio)", "Antiviral", "COVID-19, nucleoside analogue mutagenic antiviral", "800mg", 700.00f, "Rx" },
    { "Remdesivir (Veklury)", "Antiviral", "COVID-19, RSV, nucleoside analogue RNA polymerase inhibitor", "200mg IV", 1200.00f, "Rx" },
    { "Sofosbuvir/Velpatasvir (Epclusa)", "Antiviral", "Hepatitis C all genotypes, pangenotypic NS5B+NS5A", "400/100mg", 750.00f, "Rx" },
    { "Glecaprevir/Pibrentasvir (Mavyret)", "Antiviral", "Hepatitis C all genotypes including CKD, NS3/4A+NS5A", "300/120mg", 700.00f, "Rx" },
    { "Elbasvir/Grazoprevir (Zepatier)", "Antiviral", "Hepatitis C GT1 and GT4, NS5A+NS3/4A protease combo", "50/100mg", 680.00f, "Rx" },
    { "Sofosbuvir/Ledipasvir (Harvoni)", "Antiviral", "Hepatitis C GT1, NS5B polymerase + NS5A inhibitor", "400/90mg", 720.00f, "Rx" },
    { "Ombitasvir/Paritaprevir/Ritonavir (Technivie)", "Antiviral", "Hepatitis C GT4, NS5A+protease inhibitor combo", "25/150/100mg", 650.00f, "Rx" },
    { "Asunaprevir/Daclatasvir (historical HCV)", "Antiviral", "Hepatitis C GT1b, NS3+NS5A combination therapy", "100/60mg", 580.00f, "Rx" },
    { "Entecavir (Baraclude)", "Antiviral", "Chronic hepatitis B, guanosine nucleoside analogue NRTI", "0.5mg", 48.00f, "Rx" },
    { "Adefovir (Hepsera)", "Antiviral", "Chronic hepatitis B, adenine nucleotide analogue", "10mg", 42.00f, "Rx" },
    { "Telbivudine (Tyzeka)", "Antiviral", "Chronic hepatitis B, thymidine nucleoside analogue", "600mg", 38.00f, "Rx" },
    { "Clevudine (Levovir)", "Antiviral", "Chronic hepatitis B, L-nucleoside analogue, Asia-approved", "30mg", 35.00f, "Rx" },
    { "Baloxavir Marboxil (Xofluza)", "Antiviral", "Influenza A and B, cap-dependent endonuclease inhibitor", "40mg", 95.00f, "Rx" },
    { "Peramivir (Rapivab)", "Antiviral", "Influenza, IV neuraminidase inhibitor single dose", "600mg IV", 550.00f, "Rx" },
    { "Amantadine (Symmetrel)", "Antiviral", "Influenza A (historical), Parkinson's NMDA antagonist", "100mg", 12.00f, "Rx" },
    { "Ribavirin (Copegus)", "Antiviral", "HCV, RSV, purine nucleoside analogue broad antiviral", "200mg", 45.00f, "Rx" },
    { "Secukinumab (Cosentyx)", "Immune", "Psoriasis, PsA, AS, IL-17A monoclonal antibody", "300mg SC", 1400.00f, "Rx" },
    { "Brodalumab (Siliq)", "Skin", "Plaque psoriasis, IL-17RA receptor antagonist", "210mg SC", 1350.00f, "Rx" },
    { "Sarilumab (Kevzara)", "Immune", "Rheumatoid arthritis, IL-6R monoclonal antibody", "200mg SC", 1200.00f, "Rx" },
    { "Siltuximab (Sylvant)", "Immune", "Castleman disease, IL-6 monoclonal antibody", "11mg/kg IV", 1800.00f, "Rx" },
    { "Canakinumab (Ilaris)", "Immune", "Still disease, CAPS, gout flare, IL-1beta antibody", "150mg SC", 5200.00f, "Rx" },
    { "Anakinra (Kineret)", "Immune", "RA and Still disease, IL-1Ra recombinant antagonist", "100mg SC", 1200.00f, "Rx" },
    { "Rilonacept (Arcalyst)", "Immune", "CAPS and recurrent pericarditis, IL-1 trap fusion protein", "160mg SC", 2400.00f, "Rx" },
    { "Ixekizumab (Taltz)", "Immune", "Ankylosing spondylitis and PsA, IL-17A selective", "80mg SC", 1350.00f, "Rx" },
    { "Bimekizumab (Bimzelx-AS)", "Immune", "Ankylosing spondylitis, IL-17A and IL-17F inhibitor", "160mg SC", 1500.00f, "Rx" },
    { "Certolizumab Pegol (Cimzia)", "Immune", "RA, PsA, AS, CD, PsO, pegylated anti-TNF Fab", "200mg SC", 1300.00f, "Rx" },
    { "Golimumab (Simponi)", "Immune", "RA, AS, PsA, UC, monthly anti-TNF monoclonal antibody", "50mg SC", 1400.00f, "Rx" },
    { "Tocilizumab (Actemra)", "Immune", "RA, giant cell arteritis, CRS, IL-6R antibody IV/SC", "8mg/kg IV", 1300.00f, "Rx" },
    { "Satralizumab (Enspryng-RA use)", "Immune", "RA and NMOSD, IL-6R recycling antibody", "120mg SC", 1800.00f, "Rx" },
    { "Anifrolumab (Saphnelo)", "Immune", "Systemic lupus erythematosus, type I IFN receptor antibody", "300mg IV", 2200.00f, "Rx" },
    { "Belimumab (Benlysta)", "Immune", "SLE and lupus nephritis, BLyS/BAFF antibody", "10mg/kg IV", 1800.00f, "Rx" },
    { "Voclosporin (Lupkynis)", "Immune", "Lupus nephritis, novel calcineurin inhibitor", "23.7mg", 2800.00f, "Rx" },
    { "Obinutuzumab (Gazyva)", "Immune", "CLL and follicular lymphoma, glycoengineered anti-CD20", "1000mg IV", 2400.00f, "Rx" },
    { "Ublituximab (Briumvi-MS)", "Immune", "Relapsing MS, anti-CD20 with rapid B-cell depletion", "150mg IV", 1800.00f, "Rx" },
    { "Naxitamab (Danyelza)", "Oncology", "High-risk neuroblastoma, anti-GD2 antibody with GM-CSF", "3mg/kg IV", 2600.00f, "Rx" },
    { "Dinutuximab (Unituxin)", "Oncology", "High-risk neuroblastoma, anti-GD2 monoclonal antibody", "17.5mg/m2 IV", 2400.00f, "Rx" },
    { "Olaratumab (Lartruvo-restored)", "Oncology", "Soft tissue sarcoma PDGFRA antibody combination", "15mg/kg IV", 1500.00f, "Rx" },
    { "Teprotumumab (Tepezza)", "Immune", "Thyroid eye disease, IGF-1R antibody", "10mg/kg IV", 3800.00f, "Rx" },
    { "Eptinezumab-jjmr (Vyepti-alt)", "Neurological", "Chronic migraine prevention, 300mg IV quarterly anti-CGRP", "300mg IV", 850.00f, "Rx" },
    { "Calcitonin Gene-Related Peptide Antagonist", "Neurological", "Acute migraine class overview, gepant mechanism", "various", 80.00f, "Rx" },
    { "Ubrogepant (Ubrelvy)", "Neurological", "Acute migraine, oral CGRP receptor antagonist", "50mg", 75.00f, "Rx" },
    { "Ozanimod (Zeposia-UC)", "Immune", "Ulcerative colitis, S1P1/5 modulator, oral", "0.92mg", 1400.00f, "Rx" },
    { "Etrasimod (Velsipity)", "Immune", "Ulcerative colitis, S1P1/4/5 receptor modulator", "2mg", 1300.00f, "Rx" },
    { "Vedolizumab (Entyvio)", "Immune", "UC and CD, gut-selective anti-integrin alpha4beta7", "300mg IV", 1600.00f, "Rx" },
    { "Natalizumab (Tysabri)", "Immune", "Crohn's and MS, anti-alpha4 integrin antibody, JC risk", "300mg IV", 1900.00f, "Rx" },
    { "Ozanimod (Zeposia-CD)", "Immune", "Crohn's disease, S1P receptor modulator in clinical use", "0.92mg", 1400.00f, "Rx" },
    { "Ustekinumab (Stelara)", "Immune", "Crohn's, UC, psoriasis, PsA, anti-IL-12/23 p40 antibody", "45mg SC", 1500.00f, "Rx" },
    { "Brazikumab", "Immune", "Crohn's disease, anti-IL-23 specific antibody in trials", "200mg SC", 1400.00f, "Rx" },
    { "Skuselimab", "Immune", "Systemic lupus, anti-BDCA2 plasmacytoid DC targeting", "various SC", 1600.00f, "Rx" },
    { "Ixekizumab (Taltz-CD)", "Immune", "Crohn's disease exploratory use, IL-17A inhibitor", "80mg SC", 1350.00f, "Rx" },
    { "Apremilast (Otezla)", "Immune", "Psoriasis, PsA, oral aphthous ulcers, PDE4 inhibitor", "30mg", 750.00f, "Rx" },
    { "Clobetasol Propionate (Temovate)", "Skin", "Severe psoriasis, eczema, ultra-high potency steroid", "0.05%", 18.00f, "Rx" },
    { "Betamethasone Dipropionate (Diprolene)", "Skin", "Inflammatory dermatoses, ultra-high potency steroid", "0.05%", 15.00f, "Rx" },
    { "Halobetasol Propionate (Ultravate)", "Skin", "Plaque psoriasis, ultra-high potency corticosteroid", "0.05%", 18.00f, "Rx" },
    { "Diflorasone Diacetate (Psorcon)", "Skin", "Corticosteroid-responsive dermatoses, high potency steroid", "0.05%", 16.00f, "Rx" },
    { "Mometasone Furoate (Elocon)", "Skin", "Inflammatory skin conditions, mid-high potency steroid", "0.1%", 14.00f, "Rx" },
    { "Triamcinolone Acetonide (Kenalog)", "Skin", "Dermatoses and arthritis, mid-potency steroid injectable", "0.1%", 12.00f, "Rx" },
    { "Hydrocortisone Valerate (Westcort)", "Skin", "Atopic dermatitis, low-mid potency steroid", "0.2%", 10.00f, "OTC" },
    { "Desonide (DesOwen)", "Skin", "Mild atopic dermatitis and seborrheic, low-potency steroid", "0.05%", 18.00f, "Rx" },
    { "Pimecrolimus (Elidel)", "Skin", "Atopic dermatitis, topical calcineurin inhibitor, non-steroid", "1%", 28.00f, "Rx" },
    { "Tacrolimus Topical (Protopic)", "Skin", "Atopic dermatitis, topical CNI, steroid-sparing", "0.1%", 32.00f, "Rx" },
    { "Crisaborole (Eucrisa)", "Skin", "Atopic dermatitis, topical PDE4 inhibitor", "2%", 38.00f, "Rx" },
    { "Tapinarof (Vtama)", "Skin", "Plaque psoriasis and atopic dermatitis, AhR agonist", "1%", 52.00f, "Rx" },
    { "Roflumilast Cream (Zoryve)", "Skin", "Plaque psoriasis and seborrheic dermatitis, topical PDE4", "0.3%", 45.00f, "Rx" },
    { "Tralokinumab (Adbry)", "Skin", "Atopic dermatitis, IL-13 specific monoclonal antibody", "300mg SC", 1200.00f, "Rx" },
    { "Lebrikizumab (Ebglyss)", "Skin", "Atopic dermatitis, IL-13 antibody once-monthly dosing", "250mg SC", 1200.00f, "Rx" },
    { "Nemolizumab (Nemluvio)", "Skin", "Atopic dermatitis and prurigo nodularis, IL-31RA antibody", "60mg SC", 1100.00f, "Rx" },
    { "Tezepelumab (Tezspire)", "Respiratory", "Severe asthma, anti-TSLP antibody, broad mechanism", "210mg SC", 2200.00f, "Rx" },
    { "Benralizumab (Fasenra)", "Respiratory", "Severe eosinophilic asthma, anti-IL-5Ra antibody", "30mg SC", 1800.00f, "Rx" },
    { "Mepolizumab (Nucala)", "Respiratory", "Eosinophilic asthma and EGPA, anti-IL-5 antibody", "100mg SC", 1700.00f, "Rx" },
    { "Reslizumab (Cinqair)", "Respiratory", "Severe eosinophilic asthma, IV anti-IL-5 antibody", "3mg/kg IV", 1600.00f, "Rx" },
    { "Itepekimab (Dupixent-comp)", "Respiratory", "Asthma, anti-IL-33 antibody, REGN3500", "300mg SC", 1400.00f, "Rx" },
    { "Astegolimab", "Respiratory", "Eosinophilic asthma, anti-IL-33 antibody", "490mg SC", 1500.00f, "Rx" },
    { "Cendakimab", "Respiratory", "Eosinophilic esophagitis, oral IL-33R antagonist", "360mg", 1200.00f, "Rx" },
    { "Budesonide/Formoterol (Symbicort)", "Respiratory", "Asthma and COPD, ICS/LABA combination inhaler", "80/4.5mcg", 185.00f, "Rx" },
    { "Fluticasone/Salmeterol (Advair)", "Respiratory", "Asthma and COPD, ICS/LABA diskus or HFA", "100/50mcg", 195.00f, "Rx" },
    { "Fluticasone/Vilanterol (Breo)", "Respiratory", "COPD and asthma, once-daily ICS/LABA Ellipta", "100/25mcg", 245.00f, "Rx" },
    { "Mometasone/Formoterol (Dulera)", "Respiratory", "Asthma, ICS/LABA combination MDI", "100/5mcg", 200.00f, "Rx" },
    { "Beclomethasone/Formoterol (Fostair)", "Respiratory", "Asthma ICS/LABA extrafine combination", "100/6mcg", 185.00f, "Rx" },
    { "Fluticasone/Umeclidinium/Vilanterol (Trelegy)", "Respiratory", "COPD triple therapy ICS/LAMA/LABA", "100/62.5/25mcg", 285.00f, "Rx" },
    { "Budesonide/Glycopyrrolate/Formoterol (Breztri)", "Respiratory", "COPD triple therapy ICS/LAMA/LABA", "160/9/4.8mcg", 280.00f, "Rx" },
    { "Aclidinium/Formoterol (Duaklir)", "Respiratory", "COPD, LAMA/LABA dual bronchodilator", "400/12mcg", 185.00f, "Rx" },
    { "Tiotropium/Olodaterol (Stiolto)", "Respiratory", "COPD, LAMA/LABA once-daily Respimat", "2.5/2.5mcg", 280.00f, "Rx" },
    { "Umeclidinium/Vilanterol (Anoro)", "Respiratory", "COPD, LAMA/LABA once-daily Ellipta", "62.5/25mcg", 265.00f, "Rx" },
    { "Glycopyrrolate/Formoterol (Bevespi)", "Respiratory", "COPD, LAMA/LABA twice-daily Aerosphere", "9/4.8mcg", 270.00f, "Rx" },
    { "Indacaterol/Glycopyrrolate (Utibron)", "Respiratory", "COPD, LABA/LAMA twice-daily Neohaler", "27.5/15.6mcg", 260.00f, "Rx" },
    { "Ivacaftor (Kalydeco)", "Respiratory", "CF with gating mutations, CFTR potentiator", "150mg", 25000.00f, "Rx" },
    { "Lumacaftor/Ivacaftor (Orkambi)", "Respiratory", "CF homozygous F508del, CFTR corrector/potentiator", "200/125mg", 22000.00f, "Rx" },
    { "Tezacaftor/Ivacaftor (Symdeko)", "Respiratory", "CF F508del, next-gen CFTR corrector/potentiator", "100/150mg", 24000.00f, "Rx" },
    { "Elexacaftor/Tezacaftor/Ivacaftor (Trikafta)", "Respiratory", "CF triple therapy, most patients eligible", "200/100/150mg", 28000.00f, "Rx" },
    { "Ataluren (Translarna)", "Respiratory", "CF nonsense mutation, read-through agent", "10mg/kg", 1800.00f, "Rx" },
    { "Dornase Alfa (Pulmozyme)", "Respiratory", "CF mucus thinning, DNase enzyme inhalation", "2.5mg inhaled", 1200.00f, "Rx" },
    { "Hypertonic Saline 7% (HyperSal)", "Respiratory", "CF airway clearance, osmotic agent inhaled", "4mL inhaled", 28.00f, "Rx" },
    { "Amifampridine (Firdapse)", "Neurological", "Lambert-Eaton myasthenic syndrome, voltage-gated calcium channel", "10mg", 1200.00f, "Rx" },
    { "Edaravone (Radicava)", "Neurological", "ALS, free radical scavenger oxidative stress reduction", "60mg IV", 1100.00f, "Rx" },
    { "Riluzole (Rilutek)", "Neurological", "ALS, glutamate release inhibitor first approved ALS drug", "50mg", 380.00f, "Rx" },
    { "Sodium Phenylbutyrate/Ursodoxicoltaurine (Relyvrio)", "Neurological", "ALS, mitochondrial and ER stress reduction combo", "3g sachet", 158000.00f, "Rx" },
    { "Tofersen (Qalsody)", "Neurological", "SOD1-ALS, antisense oligonucleotide intrathecal injection", "100mg IT", 180000.00f, "Rx" },
    { "Pyridostigmine (Mestinon)", "Neurological", "Myasthenia gravis, acetylcholinesterase inhibitor oral", "60mg", 18.00f, "Rx" },
    { "Efgartigimod (Argenx)", "Neurological", "Generalized myasthenia gravis, FcRn blocker reduces IgG", "10mg/kg IV", 3200.00f, "Rx" },
    { "Rozanolixizumab (Rystiggo)", "Neurological", "Generalized myasthenia gravis, FcRn inhibitor SC", "7mg/kg SC", 3000.00f, "Rx" },
    { "Zilucoplan (Zilbrysq)", "Neurological", "Generalized myasthenia gravis, complement C5 inhibitor SC", "0.3mg/kg SC", 2800.00f, "Rx" },
    { "Nipocalimab", "Neurological", "Myasthenia gravis, anti-FcRn antibody reducing IgG", "30mg/kg IV", 3200.00f, "Rx" },
    { "Ravulizumab (Ultomiris-NMOSD)", "Neurological", "NMOSD AQP4+, long-acting anti-C5 antibody 8-weekly", "3000mg IV", 7500.00f, "Rx" },
    { "Opicinumab", "Neurological", "MS remyelination trial, anti-LINGO-1 antibody", "research dose IV", 0.00f, "Rx" },
    { "Ozanezumab", "Neurological", "ALS trial, anti-Nogo-A antibody", "research dose IV", 0.00f, "Rx" },
    { "Omaveloxolone (Skyclarys)", "Neurological", "Friedreich's ataxia, Nrf2 activator first approved therapy", "150mg", 340000.00f, "Rx" },
    { "Deutetrabenazine (Austedo-HD)", "Neurological", "Huntington's disease chorea, deuterated tetrabenazine VMAT2", "12mg", 1300.00f, "Rx" },
    { "Pridopidine", "Neurological", "Huntington's disease, sigma-1 receptor agonist in trials", "45mg", 280.00f, "Rx" },
    { "Tominersen", "Neurological", "Huntington's disease trial, HTT antisense oligonucleotide", "60mg IT", 0.00f, "Rx" },
    { "Sotatercept (Winrevair)", "Cardiovascular", "Pulmonary arterial hypertension, activin-ACVR2A trap", "0.7mg/kg SC", 3200.00f, "Rx" },
    { "Iloprost (Ventavis)", "Cardiovascular", "Pulmonary arterial hypertension, inhaled prostacyclin analogue", "10mcg inhaled", 1800.00f, "Rx" },
    { "Sildenafil IV (Revatio IV)", "Cardiovascular", "Pulmonary arterial hypertension IV PDE5 inhibitor", "10mg IV", 450.00f, "Rx" },
    { "Treprostinil SC (Remodulin)", "Cardiovascular", "PAH, continuous subcutaneous/IV prostacyclin analogue", "various SC/IV", 2800.00f, "Rx" },
    { "Treprostinil Oral (Orenitram)", "Cardiovascular", "PAH, oral extended-release treprostinil prostacyclin", "0.125mg", 2600.00f, "Rx" },
    { "Treprostinil Inhaled (Tyvaso)", "Cardiovascular", "PAH, inhaled treprostinil prostacyclin 4x daily", "54mcg inhaled", 2400.00f, "Rx" },
    { "Epoprostenol (Flolan)", "Cardiovascular", "PAH, continuous IV prostacyclin, short half-life", "various IV", 2200.00f, "Rx" },
    { "Ivabradine (Corlanor)", "Cardiovascular", "Heart failure with sinus tachycardia, If channel blocker", "5mg", 280.00f, "Rx" },
    { "Empagliflozin (Jardiance HF)", "Cardiovascular", "Heart failure with reduced and preserved EF, SGLT2i", "10mg", 145.00f, "Rx" },
    { "Dapagliflozin (Farxiga HF)", "Cardiovascular", "Heart failure HFrEF and HFpEF, SGLT2i", "10mg", 140.00f, "Rx" },
    { "Vericiguat (Verquvo)", "Cardiovascular", "Worsening heart failure, sGC stimulator", "10mg", 380.00f, "Rx" },
    { "Omecamtiv Mecarbil", "Cardiovascular", "Heart failure, cardiac myosin activator in trials", "25mg", 350.00f, "Rx" },
    { "Patisiran (Onpattro)", "Cardiovascular", "Hereditary transthyretin amyloidosis, first siRNA therapy IV", "0.3mg/kg IV", 42000.00f, "Rx" },
    { "Vutrisiran (Amvuttra)", "Cardiovascular", "hATTR amyloidosis, quarterly SC siRNA GalNAc conjugate", "25mg SC", 45000.00f, "Rx" },
    { "Tafamidis (Vyndaqel)", "Cardiovascular", "Transthyretin cardiomyopathy, TTR kinetic stabilizer", "61mg", 220000.00f, "Rx" },
    { "Inotersen (Tegsedi)", "Cardiovascular", "hATTR polyneuropathy, antisense oligonucleotide SC", "284mg SC", 48000.00f, "Rx" },
    { "Eplontersen (Wainua)", "Cardiovascular", "hATTR polyneuropathy, GalNAc-ASO monthly SC injection", "45mg SC", 50000.00f, "Rx" },
    { "Flecainide (Tambocor)", "Cardiovascular", "Paroxysmal AF and SVT, class IC antiarrhythmic", "100mg", 18.00f, "Rx" },
    { "Propafenone (Rythmol)", "Cardiovascular", "AF, SVT, class IC antiarrhythmic with beta-blocker", "150mg", 22.00f, "Rx" },
    { "Dofetilide (Tikosyn)", "Cardiovascular", "AF and flutter, class III pure IKr blocker", "250mcg", 45.00f, "Rx" },
    { "Ibutilide (Corvert)", "Cardiovascular", "AF cardioversion, IV class III antiarrhythmic", "1mg IV", 85.00f, "Rx" },
    { "Sotalol (Betapace)", "Cardiovascular", "AF and VT, class II/III beta-blocker/IKr inhibitor", "80mg", 18.00f, "Rx" },
    { "Quinidine Gluconate", "Cardiovascular", "AF, class IA antiarrhythmic, historical first-line", "324mg", 12.00f, "Rx" },
    { "Procainamide", "Cardiovascular", "AF, VT, class IA antiarrhythmic IV", "500mg IV", 22.00f, "Rx" },
    { "Disopyramide (Norpace)", "Cardiovascular", "AF and hypertrophic cardiomyopathy, class IA antiarrhythmic", "100mg", 18.00f, "Rx" },
    { "Mexiletine (Mexitil)", "Cardiovascular", "Ventricular arrhythmia, class IB oral antiarrhythmic", "150mg", 28.00f, "Rx" },
    { "Lidocaine IV (Xylocaine)", "Cardiovascular", "VT/VF, class IB IV antiarrhythmic", "100mg IV", 12.00f, "Rx" },
    { "Dronedarone (Multaq)", "Cardiovascular", "AF rhythm control, multi-channel blocker class III", "400mg", 185.00f, "Rx" },
    { "Vernakalant (Brinavess)", "Cardiovascular", "Rapid AF cardioversion, atrial-selective ion channel blocker", "3mg/kg IV", 245.00f, "Rx" },
    { "Etripamil Nasal (Cardamyst)", "Cardiovascular", "Paroxysmal SVT, self-administered nasal non-DHP CCB", "70mg nasal", 195.00f, "Rx" },
    { "Inclisiran (Leqvio)", "Cardiovascular", "LDL lowering, siRNA targeting PCSK9 liver synthesis", "284mg SC", 3400.00f, "Rx" },
    { "Alirocumab (Praluent)", "Cardiovascular", "LDL lowering, anti-PCSK9 monoclonal antibody SC", "75mg SC", 580.00f, "Rx" },
    { "Evolocumab (Repatha)", "Cardiovascular", "LDL lowering and CV risk reduction, anti-PCSK9 antibody", "140mg SC", 570.00f, "Rx" },
    { "Bempedoic Acid (Nexletol)", "Cardiovascular", "LDL lowering, ATP-citrate lyase inhibitor, statin-intolerant", "180mg", 280.00f, "Rx" },
    { "Ezetimibe/Bempedoic Acid (Nexlizet)", "Cardiovascular", "Combined LDL reduction, NPC1L1 + ACL inhibition", "10/180mg", 310.00f, "Rx" },
    { "Omega-3 Carboxylic Acids (Epanova)", "Cardiovascular", "Severe hypertriglyceridemia, free fatty acid omega-3", "4g", 85.00f, "Rx" },
    { "Icosapentaenoic Acid (Vascepa)", "Cardiovascular", "CV risk reduction high TG, pure EPA omega-3", "4g", 180.00f, "Rx" },
    { "Colesevelam (Welchol)", "Cardiovascular", "LDL lowering and T2DM, bile acid sequestrant", "625mg", 38.00f, "Rx" },
    { "Colestipol (Colestid)", "Cardiovascular", "LDL lowering, bile acid sequestrant granules", "1g", 28.00f, "Rx" },
    { "Cholestyramine (Questran)", "Cardiovascular", "LDL lowering, bile acid binding resin", "4g packet", 24.00f, "Rx" },
    { "Lomitapide (Juxtapid)", "Cardiovascular", "Homozygous FH, microsomal TG transfer protein inhibitor", "5mg", 32000.00f, "Rx" },
    { "Mipomersen (Kynamro)", "Cardiovascular", "Homozygous FH, ApoB-100 antisense oligonucleotide", "200mg SC", 28000.00f, "Rx" },
    { "Evinprokimab", "Cardiovascular", "LDL lowering, anti-ANGPTL3 antibody in trials", "various IV", 0.00f, "Rx" },
    { "Ticagrelor (Brilinta)", "Cardiovascular", "Acute coronary syndrome, P2Y12 reversible ADP antagonist", "90mg", 180.00f, "Rx" },
    { "Prasugrel (Effient)", "Cardiovascular", "ACS PCI, P2Y12 irreversible thienopyridine prodrug", "10mg", 185.00f, "Rx" },
    { "Cangrelor (Kengreal)", "Cardiovascular", "PCI ACS, IV P2Y12 reversible fast-onset/offset", "30mcg/kg IV", 380.00f, "Rx" },
    { "Vorapaxar (Zontivity)", "Cardiovascular", "CV event reduction, PAR-1 thrombin receptor antagonist", "2.08mg", 245.00f, "Rx" },
    { "Bivalirudin (Angiomax)", "Cardiovascular", "ACS and PCI, direct thrombin inhibitor, hirudin analogue", "0.75mg/kg IV", 380.00f, "Rx" },
    { "Argatroban", "Cardiovascular", "HIT, hepatic-metabolized direct thrombin inhibitor IV", "2mcg/kg/min IV", 280.00f, "Rx" },
    { "Dabigatran (Pradaxa)", "Blood", "AF stroke prevention and DVT/PE, oral direct thrombin inhibitor", "150mg", 185.00f, "Rx" },
    { "Edoxaban (Savaysa)", "Blood", "AF and VTE, factor Xa inhibitor once-daily", "60mg", 180.00f, "Rx" },
    { "Betrixaban (Bevyxxa)", "Blood", "VTE prophylaxis hospital medical, factor Xa inhibitor", "80mg", 195.00f, "Rx" },
    { "Desirudin (Iprivask)", "Blood", "DVT prophylaxis hip replacement, direct thrombin inhibitor SC", "15mg SC", 280.00f, "Rx" },
    { "Fondaparinux (Arixtra)", "Blood", "DVT/PE and ACS prophylaxis/treatment, anti-Xa pentasaccharide", "2.5mg SC", 45.00f, "Rx" },
    { "Andexanet Alfa (Andexxa)", "Blood", "Factor Xa inhibitor reversal, modified Xa decoy protein", "400mg IV", 25000.00f, "Rx" },
    { "Idarucizumab (Praxbind)", "Blood", "Dabigatran reversal, humanized Fab antibody fragment", "5g IV", 4200.00f, "Rx" },
    { "Ciraparantag", "Blood", "Universal anticoagulant reversal in development", "100mg IV", 0.00f, "Rx" },
    { "Eptifibatide (Integrilin)", "Blood", "ACS and PCI, cyclic peptide GP IIb/IIIa inhibitor", "180mcg/kg IV", 280.00f, "Rx" },
    { "Tirofiban (Aggrastat)", "Blood", "ACS PCI, non-peptide GP IIb/IIIa inhibitor IV", "25mcg/kg IV", 260.00f, "Rx" },
    { "Abciximab (ReoPro)", "Blood", "PCI, chimeric GP IIb/IIIa monoclonal antibody Fab", "0.25mg/kg IV", 290.00f, "Rx" },
    { "Oxymorphone (Numorphan)", "Pain Relief", "Moderate to severe pain, potent semi-synthetic opioid", "10mg", 95.00f, "Rx" },
    { "Hydromorphone (Dilaudid)", "Pain Relief", "Severe pain, semi-synthetic opioid 5-8x morphine potency", "4mg", 28.00f, "Rx" },
    { "Methadone (Dolophine)", "Pain Relief", "Severe pain and opioid use disorder maintenance, NMDA", "10mg", 18.00f, "Rx" },
    { "Tapentadol (Nucynta ER)", "Pain Relief", "Chronic moderate-severe pain, MOR agonist and NRI", "100mg", 165.00f, "Rx" },
    { "Sufentanil Sublingual (Dsuvia)", "Pain Relief", "Acute severe pain in monitored settings, potent opioid", "30mcg SL", 38.00f, "Rx" },
    { "Levorphanol (Levo-Dromoran)", "Pain Relief", "Severe pain, left-handed morphine isomer NMDA antagonist", "2mg", 45.00f, "Rx" },
    { "Pentazocine (Talwin)", "Pain Relief", "Moderate pain, mixed opioid agonist-antagonist kappa/sigma", "50mg", 28.00f, "Rx" },
    { "Nalbuphine (Nubain)", "Pain Relief", "Moderate pain, kappa opioid agonist and mu antagonist", "10mg IM", 22.00f, "Rx" },
    { "Butorphanol (Stadol)", "Pain Relief", "Migraine and pain, opioid agonist-antagonist intranasal", "1mg/spray", 28.00f, "Rx" },
    { "Dezocine (Dalgan)", "Pain Relief", "Moderate to severe pain, partial opioid agonist", "10mg", 35.00f, "Rx" },
    { "Tetracaine (Pontocaine)", "Pain Relief", "Spinal anesthesia and topical, amino ester local anesthetic", "10mg/mL", 18.00f, "Rx" },
    { "Mepivacaine (Carbocaine)", "Pain Relief", "Dental/regional anesthesia, intermediate amide local anesthetic", "2%", 14.00f, "Rx" },
    { "Prilocaine (Citanest)", "Pain Relief", "Dental anesthesia and EMLA component, amide local anesthetic", "4%", 12.00f, "Rx" },
    { "Chloroprocaine (Nesacaine)", "Pain Relief", "Epidural anesthesia, rapid-acting amino ester, short duration", "2%", 12.00f, "Rx" },
    { "Phenazopyridine (Pyridium)", "Pain Relief", "UTI urinary tract pain relief, azo dye analgesic", "200mg", 9.99f, "OTC" },
    { "Sumatriptan SC (Imitrex SC)", "Neurological", "Acute migraine and cluster headache, subcutaneous triptan", "6mg SC", 55.00f, "Rx" },
    { "Sumatriptan Nasal (Imitrex Nasal)", "Neurological", "Acute migraine, intranasal triptan formulation", "20mg nasal", 48.00f, "Rx" },
    { "Rizatriptan (Maxalt)", "Neurological", "Acute migraine, oral triptan 5-HT1B/1D agonist", "10mg", 38.00f, "Rx" },
    { "Almotriptan (Axert)", "Neurological", "Acute migraine, oral triptan with good tolerability", "12.5mg", 38.00f, "Rx" },
    { "Frovatriptan (Frova)", "Neurological", "Acute migraine and menstrual migraine prevention, long-acting triptan", "2.5mg", 38.00f, "Rx" },
    { "Naratriptan (Amerge)", "Neurological", "Acute migraine, oral triptan with slower onset less headache recurrence", "2.5mg", 38.00f, "Rx" },
    { "Eletriptan (Relpax)", "Neurological", "Acute migraine, potent oral triptan with high receptor affinity", "40mg", 42.00f, "Rx" },
    { "Dihydroergotamine Nasal (Migranal)", "Neurological", "Acute migraine, ergot alkaloid nasal spray", "0.5mg/spray", 95.00f, "Rx" },
    { "Dihydroergotamine IV (DHE-45)", "Neurological", "Acute migraine IV ergot, serotonin/adrenergic vasoconstrictor", "1mg IV", 85.00f, "Rx" },
    { "Oxybutynin (Ditropan)", "Urology", "Overactive bladder, antimuscarinic M1/M2/M3 blocker", "5mg", 12.00f, "Rx" },
    { "Tolterodine (Detrol)", "Urology", "OAB, bladder-selective M2/M3 antimuscarinic", "2mg", 28.00f, "Rx" },
    { "Darifenacin (Enablex)", "Urology", "OAB, M3-selective antimuscarinic, less CNS effects", "7.5mg", 32.00f, "Rx" },
    { "Solifenacin (Vesicare)", "Urology", "OAB, M3-selective antimuscarinic once daily", "5mg", 28.00f, "Rx" },
    { "Fesoterodine (Toviaz)", "Urology", "OAB, prodrug of tolterodine active metabolite", "4mg", 30.00f, "Rx" },
    { "Trospium (Sanctura)", "Urology", "OAB, quaternary ammonium antimuscarinic, less CNS", "20mg", 28.00f, "Rx" },
    { "Mirabegron (Myrbetriq)", "Urology", "OAB, beta-3 adrenergic receptor agonist, first in class", "25mg", 85.00f, "Rx" },
    { "Vibegron (Gemtesa)", "Urology", "OAB, highly selective beta-3 adrenergic agonist", "75mg", 88.00f, "Rx" },
    { "Imidafenacin (Uritos)", "Urology", "OAB, M1/M3 selective antimuscarinic, Japan-approved", "0.1mg", 28.00f, "Rx" },
    { "Tamsulosin (Flomax)", "Urology", "BPH, alpha-1A selective blocker for urine flow", "0.4mg", 12.00f, "Rx" },
    { "Alfuzosin (Uroxatral)", "Urology", "BPH, alpha-1 selective blocker, low orthostatic hypotension", "10mg", 18.00f, "Rx" },
    { "Silodosin (Rapaflo)", "Urology", "BPH, highly alpha-1A selective blocker for LUTS", "8mg", 22.00f, "Rx" },
    { "Finasteride (Proscar)", "Urology", "BPH and male pattern baldness, 5alpha-reductase type 2 inhibitor", "5mg", 22.00f, "Rx" },
    { "Dutasteride (Avodart)", "Urology", "BPH, dual 5alpha-reductase type 1 and 2 inhibitor", "0.5mg", 28.00f, "Rx" },
    { "Tadalafil (Adcirca/Cialis)", "Urology", "BPH, ED, and PAH, long-acting PDE5 inhibitor", "5mg", 35.00f, "Rx" },
    { "Noctiva (Desmopressin Nasal)", "Urology", "Nocturia, low-dose nasal desmopressin DDAVP formulation", "0.83mcg", 45.00f, "Rx" },
    { "Oxybutynin Patch (Oxytrol Women)", "Urology", "OAB in women, transdermal antimuscarinic patch", "3.9mg/day", 22.00f, "OTC" },
    { "Sacral Neuromodulation (InterStim)", "Urology", "Refractory OAB, implantable sacral nerve modulation device", "device", 28000.00f, "Rx" },
    { "Lidocaine/Prilocaine (EMLA)", "Skin", "Topical anesthesia, eutectic mixture local anesthetic cream", "2.5/2.5%", 18.00f, "Rx" },
    { "Benzoyl Peroxide (PanOxyl)", "Skin", "Acne vulgaris, oxidizing bactericidal keratolytic agent", "5%", 8.99f, "OTC" },
    { "Tretinoin (Retin-A)", "Skin", "Acne and photodamage, topical retinoid vitamin A acid", "0.025%", 22.00f, "Rx" },
    { "Adapalene (Differin)", "Skin", "Acne, synthetic retinoid with fewer irritation effects", "0.1%", 12.99f, "OTC" },
    { "Tazarotene (Tazorac)", "Skin", "Psoriasis and acne, topical retinoid with high efficacy", "0.1%", 28.00f, "Rx" },
    { "Trifarotene (Aklief)", "Skin", "Acne on face and trunk, selective RARgamma retinoid", "0.005%", 32.00f, "Rx" },
    { "Clindamycin Topical (Cleocin-T)", "Skin", "Acne, topical lincosamide antibiotic", "1%", 16.00f, "Rx" },
    { "Dapsone Gel (Aczone)", "Skin", "Acne, topical sulfone anti-inflammatory antibacterial", "5%", 28.00f, "Rx" },
    { "Ivermectin Cream (Soolantra)", "Skin", "Rosacea, topical avermectin anti-Demodex antiparasitic", "1%", 38.00f, "Rx" },
    { "Azelaic Acid (Finacea)", "Skin", "Rosacea and acne, dicarboxylic acid antibacterial", "15%", 22.00f, "Rx" },
    { "Brimonidine Gel (Mirvaso)", "Skin", "Rosacea flushing, topical alpha-2 agonist vasoconstrictor", "0.33%", 28.00f, "Rx" },
    { "Metronidazole Gel (MetroGel)", "Skin", "Rosacea, topical nitroimidazole anti-inflammatory", "1%", 18.00f, "Rx" },
    { "Oxymetazoline Cream (Rhofade)", "Skin", "Rosacea erythema, topical alpha-1A/2A agonist", "1%", 32.00f, "Rx" },
    { "Nalmefene (Selincro)", "Mental Health", "Alcohol use disorder, opioid receptor partial agonist/antagonist", "18mg", 68.00f, "Rx" },
    { "Sodium Oxybate (Xyrem)", "Mental Health", "Narcolepsy cataplexy and EDS, GHB sodium salt", "4.5g", 850.00f, "Rx" },
    { "Calcium Oxybate (Lumryz)", "Mental Health", "Narcolepsy, once-nightly low-sodium oxybate formulation", "4.5g", 880.00f, "Rx" },
    { "Modafinil (Provigil)", "Mental Health", "Narcolepsy, OSA EDS, shift work, wakefulness promoter", "200mg", 380.00f, "Rx" },
    { "Armodafinil (Nuvigil)", "Mental Health", "Narcolepsy and OSA, R-enantiomer of modafinil EDS", "150mg", 350.00f, "Rx" },
    { "Tasimelteon (Hetlioz)", "Mental Health", "Non-24-hour sleep-wake disorder in blind patients, MT1/MT2 agonist", "20mg", 680.00f, "Rx" },
    { "Melatonin (Circadin)", "Mental Health", "Insomnia and jet lag, pineal hormone sleep-wake regulator", "3mg", 4.99f, "OTC" },
    { "Agomelatine (Valdoxan)", "Mental Health", "Depression, MT1/MT2 agonist and 5-HT2C antagonist", "25mg", 85.00f, "Rx" },
    { "Meclofenamate (Meclomen)", "Pain Relief", "Arthritis and dysmenorrhea, NSAID with fenamate class", "100mg", 22.00f, "Rx" },
    { "Diflunisal (Dolobid)", "Pain Relief", "Arthritis and mild pain, salicylate-based NSAID", "500mg", 18.00f, "Rx" },
    { "Etodolac (Lodine)", "Pain Relief", "Arthritis, NSAID with preferential COX-2 activity", "400mg", 18.00f, "Rx" },
    { "Fenoprofen (Nalfon)", "Pain Relief", "Arthritis and pain, propionic acid NSAID", "600mg", 18.00f, "Rx" },
    { "Flurbiprofen (Ansaid)", "Pain Relief", "Arthritis and pain, propionic acid NSAID", "100mg", 16.00f, "Rx" },
    { "Ketoprofen (Orudis)", "Pain Relief", "Arthritis and pain, propionic acid NSAID OTC/Rx", "75mg", 14.00f, "OTC" },
    { "Mefenamic Acid (Ponstel)", "Pain Relief", "Dysmenorrhea and pain, fenamate NSAID", "500mg", 22.00f, "Rx" },
    { "Oxaprozin (Daypro)", "Pain Relief", "Arthritis, long-acting propionic acid NSAID once daily", "600mg", 18.00f, "Rx" },
    { "Sulindac (Clinoril)", "Pain Relief", "Arthritis and gout, indene acetic acid NSAID prodrug", "200mg", 18.00f, "Rx" },
    { "Tolmetin (Tolectin)", "Pain Relief", "Juvenile and adult rheumatoid arthritis, NSAID", "400mg", 16.00f, "Rx" },
    { "Leflunomide (Arava)", "Immune", "Rheumatoid arthritis, DHODH inhibitor DMARD", "20mg", 28.00f, "Rx" },
    { "Hydroxychloroquine (Plaquenil)", "Immune", "RA, SLE, malaria, aminoquinoline antimalarial/DMARD", "200mg", 18.00f, "Rx" },
    { "Sulfasalazine (Azulfidine)", "Immune", "RA, IBD, and AS, sulfonamide/salicylate DMARD", "500mg", 16.00f, "Rx" },
    { "Penicillamine (Cuprimine)", "Immune", "RA and Wilson disease, metal chelator DMARD", "250mg", 22.00f, "Rx" },
    { "Gold Sodium Thiomalate (Myochrysine)", "Immune", "Rheumatoid arthritis, injectable gold salt DMARD", "50mg IM", 145.00f, "Rx" },
    { "Auranofin (Ridaura)", "Immune", "Rheumatoid arthritis, oral gold compound DMARD", "3mg", 95.00f, "Rx" },
    { "Tofacitinib (Xeljanz)", "Immune", "RA, PsA, AS, UC, pan-JAK inhibitor oral DMARD", "5mg", 850.00f, "Rx" },
    { "Tacrolimus Oral (Prograf)", "Immune", "Organ transplant rejection, calcineurin inhibitor", "1mg", 85.00f, "Rx" },
    { "Cyclosporine (Neoral)", "Immune", "Transplant, RA, and psoriasis, calcineurin inhibitor", "100mg", 95.00f, "Rx" },
    { "Mycophenolate Mofetil (CellCept)", "Immune", "Transplant rejection, IMPDH inhibitor antiproliferative", "500mg", 45.00f, "Rx" },
    { "Mycophenolic Acid (Myfortic)", "Immune", "Transplant, enteric-coated MPA formulation IMPDH inhibitor", "360mg", 55.00f, "Rx" },
    { "Azathioprine (Imuran)", "Immune", "Transplant and autoimmune, thiopurine antimetabolite DMARD", "50mg", 28.00f, "Rx" },
    { "Everolimus (Zortress)", "Immune", "Organ transplant, mTOR inhibitor antiproliferative", "0.75mg", 285.00f, "Rx" },
    { "Sirolimus (Rapamune)", "Immune", "Renal transplant and LAM, mTOR inhibitor", "1mg", 245.00f, "Rx" },
    { "Belatacept (Nulojix)", "Immune", "Renal transplant, CTLA-4 Ig costimulation blocker", "5mg/kg IV", 1200.00f, "Rx" },
    { "Basiliximab (Simulect)", "Immune", "Acute renal transplant rejection, anti-CD25 IL-2Ra antibody", "20mg IV", 1400.00f, "Rx" },
    { "Antithymocyte Globulin (Thymoglobulin)", "Immune", "Transplant rejection induction and AA, polyclonal rabbit ATG", "1.5mg/kg IV", 1800.00f, "Rx" },
    { "Daclizumab (Zinbryta-withdrawn)", "Immune", "MS and transplant, anti-CD25 antibody, withdrawn safety", "150mg SC", 0.00f, "Rx" },
    { "Intravenous Immunoglobulin (IVIG)", "Immune", "Immunodeficiency, ITP, GBS, IVIG pooled human plasma", "1g/kg IV", 1200.00f, "Rx" },
    { "Subcutaneous Immunoglobulin (SCIG)", "Immune", "Primary immunodeficiency, home self-administration IgG", "100mg/kg SC", 1100.00f, "Rx" },
    { "Hyaluronidase/Facilitated SCIG (Hyqvia)", "Immune", "Primary immunodeficiency, enzyme-facilitated SC IgG", "600mg/5mL SC", 1250.00f, "Rx" },
    { "Complement C1 Esterase Inhibitor (Berinert)", "Immune", "Hereditary angioedema acute attack, C1-INH concentrate", "20units/kg IV", 4500.00f, "Rx" },
    { "Lanadelumab (Takhzyro)", "Immune", "HAE prevention, anti-plasma kallikrein monoclonal antibody", "300mg SC", 3800.00f, "Rx" },
    { "Berotralstat (Orladeyo)", "Immune", "HAE prevention, oral plasma kallikrein inhibitor", "150mg", 3200.00f, "Rx" },
    { "Garadacimab", "Immune", "HAE prevention, anti-factor XIIa antibody monthly SC", "200mg SC", 3500.00f, "Rx" },
    { "Icatibant (Firazyr)", "Immune", "HAE acute attack, bradykinin B2 receptor antagonist SC", "30mg SC", 1800.00f, "Rx" },
    { "Ecallantide (Kalbitor)", "Immune", "HAE acute attack, plasma kallikrein inhibitor SC", "30mg SC", 1600.00f, "Rx" },
    { "Daxdilimab", "Immune", "Cutaneous lupus, anti-ILT7 antibody", "various SC", 1600.00f, "Rx" },
    { "Iberdomide", "Immune", "SLE, cereblon E3 ligase modulator, cereblon modulator", "1.5mg", 850.00f, "Rx" },
    { "Obeticholic Acid (Ocaliva)", "Digestive", "PBC, farnesoid X receptor agonist bile acid", "5mg", 1800.00f, "Rx" },
    { "Seladelpar (Livdelzi)", "Digestive", "Primary biliary cholangitis, PPAR-delta agonist", "10mg", 1600.00f, "Rx" },
    { "Elafibranor (Iqirvo)", "Digestive", "PBC, dual PPAR-alpha/delta agonist", "80mg", 1700.00f, "Rx" },
    { "Bulevirtide (Hepcludex)", "Antiviral", "Hepatitis delta virus, HBV/HDV entry inhibitor peptide", "2mg SC", 1400.00f, "Rx" },
    { "Peginterferon Alfa-2a (Pegasys)", "Antiviral", "Hepatitis B and C, pegylated interferon SC weekly", "180mcg SC", 480.00f, "Rx" },
    { "Peginterferon Alfa-2b (PegIntron)", "Antiviral", "Hepatitis C, pegylated interferon SC weekly", "80mcg SC", 460.00f, "Rx" },
    { "Ursodeoxycholic Acid (Actigall)", "Digestive", "PBC, gallstones dissolution, bile acid liver protection", "300mg", 28.00f, "Rx" },
    { "Norursodeoxycholic Acid (NorUDCA)", "Digestive", "PSC experimental, modified bile acid anti-fibrotic", "500mg", 85.00f, "Rx" },
    { "Lanifibranor", "Digestive", "NASH/MASH, pan-PPAR agonist alpha/delta/gamma", "800mg", 350.00f, "Rx" },
    { "Resmetirom (Rezdiffra)", "Digestive", "NASH/MASH with fibrosis, thyroid hormone receptor beta agonist", "100mg", 1800.00f, "Rx" },
    { "Semaglutide (Wegovy-NASH)", "Digestive", "NASH resolution, GLP-1 agonist anti-fibrotic effect", "2.4mg SC", 1200.00f, "Rx" },
    { "Aramchol", "Digestive", "NASH, fatty liver SCD1 inhibitor conjugate in trials", "300mg", 280.00f, "Rx" },
    { "Cenicriviroc", "Digestive", "NASH fibrosis, CCR2/CCR5 dual chemokine receptor antagonist", "150mg", 320.00f, "Rx" },
    { "Simtuzumab", "Digestive", "NASH fibrosis, anti-LOXL2 antibody, collagen crosslinking", "700mg IV", 0.00f, "Rx" },
    { "Semaglutide SC (Ozempic)", "Diabetes", "T2DM and CV risk, once-weekly GLP-1 receptor agonist", "0.5mg SC", 380.00f, "Rx" },
    { "Semaglutide Oral (Rybelsus)", "Diabetes", "T2DM, first oral GLP-1 receptor agonist tablet", "7mg", 280.00f, "Rx" },
    { "Efpeglenatide (Eftreaza)", "Diabetes", "T2DM, weekly GLP-1 Fc-fusion peptide agonist", "4mg SC", 380.00f, "Rx" },
    { "Orforglipron", "Diabetes", "T2DM and obesity, oral non-peptide GLP-1 agonist small molecule", "36mg", 280.00f, "Rx" },
    { "Retatrutide", "Diabetes", "Obesity, triple GIP/GLP-1/glucagon receptor agonist", "12mg SC", 450.00f, "Rx" },
    { "Cagrilintide/Semaglutide (CagriSema)", "Diabetes", "Obesity, amylin analogue plus GLP-1 agonist combination", "various SC", 500.00f, "Rx" },
    { "Setmelanotide (Imcivree)", "Diabetes", "Obesity due to POMC/LEPR/PCSK1 deficiency, MC4R agonist", "3mg SC", 38000.00f, "Rx" },
    { "Metreleptin (Myalept)", "Diabetes", "Leptin deficiency and lipodystrophy, recombinant leptin", "5mg SC", 28000.00f, "Rx" },
    { "Exenatide ER (Bydureon)", "Diabetes", "T2DM, once-weekly exenatide extended-release microspheres", "2mg SC", 280.00f, "Rx" },
    { "Albiglutide (Tanzeum)", "Diabetes", "T2DM, weekly GLP-1 Fc albumin fusion discontinued", "30mg SC", 250.00f, "Rx" },
    { "Glyburide/Metformin (Glucovance)", "Diabetes", "T2DM, fixed-dose sulfonylurea plus biguanide combination", "2.5/500mg", 18.00f, "Rx" },
    { "Sitagliptin/Metformin (Janumet)", "Diabetes", "T2DM, DPP-4 inhibitor plus metformin combination", "50/500mg", 65.00f, "Rx" },
    { "Empagliflozin/Metformin (Synjardy)", "Diabetes", "T2DM, SGLT2 inhibitor plus metformin combination", "10/500mg", 125.00f, "Rx" },
    { "Dapagliflozin/Metformin (Xigduo)", "Diabetes", "T2DM, SGLT2 inhibitor plus metformin combination", "10/500mg", 120.00f, "Rx" },
    { "Canagliflozin/Metformin (Invokamet)", "Diabetes", "T2DM, SGLT2i and metformin combination tablet", "150/500mg", 115.00f, "Rx" },
    { "Ertugliflozin/Metformin (Segluromet)", "Diabetes", "T2DM, SGLT2 inhibitor plus metformin", "5/500mg", 120.00f, "Rx" },
    { "Insulin Icodec", "Diabetes", "T2DM, once-weekly ultra-long basal insulin analogue", "various SC", 380.00f, "Rx" },
    { "Prandial Insulin Tregopil", "Diabetes", "T2DM, oral insulin tablet with absorption enhancer India", "175units", 45.00f, "Rx" },
    { "Bromocriptine (Cycloset)", "Diabetes", "T2DM, dopamine D2 agonist morning dose glucose control", "0.8mg", 85.00f, "Rx" },
    { "Sotagliflozin (Inpefa)", "Diabetes", "T2DM and HF, dual SGLT1/SGLT2 inhibitor", "400mg", 145.00f, "Rx" },
    { "Methimazole (Tapazole)", "Hormonal", "Hyperthyroidism and Graves disease, thionamide thyroid inhibitor", "10mg", 15.00f, "Rx" },
    { "Propylthiouracil (PTU)", "Hormonal", "Hyperthyroidism, thionamide preferred in pregnancy first trimester", "50mg", 12.00f, "Rx" },
    { "Lugol's Solution", "Hormonal", "Pre-thyroid surgery preparation, potassium iodide thyroid blockade", "5% iodine", 8.00f, "Rx" },
    { "Potassium Iodide (SSKI)", "Hormonal", "Thyroid storm and radiation emergency, stable iodide blockade", "65mg", 10.00f, "OTC" },
    { "Hydrocortisone (Solu-Cortef)", "Hormonal", "Adrenal insufficiency, stress dosing, corticosteroid IV", "100mg IV", 18.00f, "Rx" },
    { "Methylprednisolone IV (Solu-Medrol)", "Hormonal", "Acute inflammation, MS relapse, IV corticosteroid", "1g IV", 25.00f, "Rx" },
    { "Dexamethasone IV (Decadron IV)", "Hormonal", "Brain edema, croup, nausea, anti-inflammatory corticosteroid", "10mg IV", 8.00f, "Rx" },
    { "Fludrocortisone (Florinef)", "Hormonal", "Adrenal insufficiency and orthostatic hypotension, mineralocorticoid", "0.1mg", 12.00f, "Rx" },
    { "Cosyntropin (Cortrosyn)", "Hormonal", "Adrenal insufficiency testing, synthetic ACTH stimulation", "250mcg IM", 85.00f, "Rx" },
    { "Pasireotide (Signifor)", "Hormonal", "Cushing disease, multi-receptor somatostatin analogue", "0.6mg SC", 2800.00f, "Rx" },
    { "Osilodrostat (Isturisa)", "Hormonal", "Cushing syndrome, 11beta-hydroxylase inhibitor CYP11B1", "2mg", 3200.00f, "Rx" },
    { "Mifepristone (Korlym)", "Hormonal", "Cushing syndrome hyperglycemia, glucocorticoid receptor blocker", "300mg", 1800.00f, "Rx" },
    { "Metyrapone (Metopirone)", "Hormonal", "Cushing syndrome, 11-beta hydroxylase enzyme inhibitor", "250mg", 380.00f, "Rx" },
    { "Ketoconazole Oral (Nizoral)", "Hormonal", "Cushing syndrome, adrenal steroidogenesis inhibitor", "200mg", 28.00f, "Rx" },
    { "Levoketoconazole (Recorlev)", "Hormonal", "Cushing syndrome, 11beta-hydroxylase inhibitor L-isomer", "150mg", 1800.00f, "Rx" },
    { "Lanreotide (Somatuline)", "Hormonal", "Acromegaly and carcinoid, long-acting somatostatin analogue", "60mg SC", 2400.00f, "Rx" },
    { "Pegvisomant (Somavert)", "Hormonal", "Acromegaly, GH receptor antagonist protein analogue", "10mg SC", 1800.00f, "Rx" },
    { "Cabergoline (Dostinex)", "Hormonal", "Hyperprolactinemia and Parkinson's, dopamine D2 agonist", "0.5mg", 18.00f, "Rx" },
    { "Quinagolide (Norprolac)", "Hormonal", "Hyperprolactinemia, non-ergot dopamine D2 agonist", "75mcg", 22.00f, "Rx" },
    { "Somatropin (Genotropin)", "Hormonal", "GH deficiency and short stature, recombinant human GH", "5mg SC", 1800.00f, "Rx" },
    { "Somapacitan (Sogroya)", "Hormonal", "GH deficiency adults, once-weekly GH analogue SC", "8mg SC", 2200.00f, "Rx" },
    { "Lonapegsomatropin (Skytrofa)", "Hormonal", "GH deficiency children, once-weekly prodrug SC injection", "varies SC", 2400.00f, "Rx" },
    { "Vosoritide (Voxzogo)", "Hormonal", "Achondroplasia in children, C-natriuretic peptide analogue", "15mcg/kg SC", 280000.00f, "Rx" },
    { "Levothyroxine IV (Synthroid IV)", "Hormonal", "Myxedema coma, IV thyroxine replacement therapy", "200mcg IV", 45.00f, "Rx" },
    { "Desiccated Thyroid (Armour Thyroid)", "Hormonal", "Hypothyroidism, porcine thyroid T3/T4 combination", "60mg", 18.00f, "Rx" },
    { "Teriparatide (Forteo)", "Blood", "Osteoporosis, PTH 1-34 anabolic bone formation stimulator", "20mcg SC", 850.00f, "Rx" },
    { "Abaloparatide (Tymlos)", "Blood", "Osteoporosis, PTHrP analogue anabolic bone agent", "80mcg SC", 880.00f, "Rx" },
    { "Romosozumab (Evenity)", "Blood", "Osteoporosis, anti-sclerostin antibody dual action bone", "210mg SC", 1800.00f, "Rx" },
    { "Denosumab (Prolia)", "Blood", "Osteoporosis and bone metastasis, anti-RANKL antibody", "60mg SC", 950.00f, "Rx" },
    { "Ibandronate (Boniva)", "Blood", "Osteoporosis, bisphosphonate monthly oral or quarterly IV", "150mg", 28.00f, "Rx" },
    { "Risedronate (Actonel)", "Blood", "Osteoporosis and Paget disease, bisphosphonate oral", "35mg weekly", 32.00f, "Rx" },
    { "Etidronate (Didronel)", "Blood", "Paget disease and hypercalcemia, first-gen bisphosphonate", "400mg", 22.00f, "Rx" },
    { "Pamidronate (Aredia)", "Blood", "Hypercalcemia of malignancy and myeloma, IV bisphosphonate", "90mg IV", 85.00f, "Rx" },
    { "Cinacalcet (Sensipar)", "Blood", "Secondary hyperparathyroidism and parathyroid cancer, calcimimetic", "30mg", 95.00f, "Rx" },
    { "Etelcalcetide (Parsabiv)", "Blood", "Secondary HPT on hemodialysis, IV calcimimetic", "5mg IV", 380.00f, "Rx" },
    { "Sodium Fluoride (Luride)", "Vitamin", "Dental caries prevention, fluoride supplementation", "0.25mg", 5.99f, "OTC" },
    { "Strontium Ranelate (Protelos)", "Blood", "Osteoporosis Europe-only, bone formation and resorption dual action", "2g sachet", 28.00f, "Rx" },
    { "Calcitonin Nasal (Miacalcin)", "Blood", "Osteoporosis and Paget disease, salmon calcitonin nasal", "200units", 38.00f, "Rx" },
    { "Atorvastatin/Amlodipine (Caduet)", "Cardiovascular", "Hypertension and hypercholesterolemia, fixed-dose combination", "10/5mg", 22.00f, "Rx" },
    { "Sacubitril/Valsartan (Entresto)", "Cardiovascular", "Heart failure reduced EF, ARNI combination", "49/51mg", 480.00f, "Rx" },
    { "Chlorthalidone (Thalitone)", "Cardiovascular", "Hypertension and edema, thiazide-like diuretic", "25mg", 8.00f, "Rx" },
    { "Indapamide (Lozol)", "Cardiovascular", "Hypertension, thiazide-like diuretic with vasodilatory effects", "1.25mg", 10.00f, "Rx" },
    { "Metolazone (Zaroxolyn)", "Cardiovascular", "Edema resistant to loop diuretics, thiazide-like diuretic", "2.5mg", 12.00f, "Rx" },
    { "Acetazolamide (Diamox)", "Cardiovascular", "Altitude sickness, glaucoma, seizures, carbonic anhydrase inhibitor", "250mg", 18.00f, "Rx" },
    { "Torsemide (Demadex)", "Cardiovascular", "Edema and heart failure, loop diuretic once-daily", "10mg", 10.00f, "Rx" },
    { "Bumetanide (Bumex)", "Cardiovascular", "Edema, potent loop diuretic 40x furosemide potency", "1mg", 10.00f, "Rx" },
    { "Ethacrynic Acid (Edecrin)", "Cardiovascular", "Edema in sulfa allergy patients, loop diuretic", "25mg", 22.00f, "Rx" },
    { "Eplerenone (Inspra)", "Cardiovascular", "Heart failure and hypertension, selective mineralocorticoid antagonist", "25mg", 35.00f, "Rx" },
    { "Finerenone (Kerendia)", "Cardiovascular", "CKD with T2DM, non-steroidal MR antagonist cardiorenal", "10mg", 380.00f, "Rx" },
    { "Amiloride (Midamor)", "Cardiovascular", "Potassium-sparing diuretic hypertension, ENaC channel blocker", "5mg", 12.00f, "Rx" },
    { "Triamterene (Dyrenium)", "Cardiovascular", "Potassium-sparing diuretic, direct ENaC channel inhibitor", "50mg", 14.00f, "Rx" },
    { "Hydralazine (Apresoline)", "Cardiovascular", "Hypertension and heart failure, direct arterial vasodilator", "25mg", 10.00f, "Rx" },
    { "Isosorbide Dinitrate (Isordil)", "Cardiovascular", "Angina and heart failure nitrate, venous dilator", "10mg", 10.00f, "Rx" },
    { "Isosorbide Mononitrate (Imdur)", "Cardiovascular", "Angina prevention, long-acting nitrate once daily", "30mg", 12.00f, "Rx" },
    { "Nitroglycerin Patch (Nitro-Dur)", "Cardiovascular", "Angina prevention, transdermal nitroglycerin nitrate", "0.2mg/hr", 14.00f, "Rx" },
    { "Nitroglycerin Spray (Nitrolingual)", "Cardiovascular", "Acute angina, sublingual nitroglycerin spray", "0.4mg/spray", 22.00f, "Rx" },
    { "Ranolazine (Ranexa)", "Cardiovascular", "Chronic angina, late sodium current inhibitor antianginal", "500mg", 185.00f, "Rx" },
    { "Trimetazidine (Vastarel)", "Cardiovascular", "Chronic angina, metabolic antianginal 3-KAT inhibitor", "35mg", 22.00f, "Rx" },
    { "Nicardipine SR (Cardene SR)", "Cardiovascular", "Hypertension, sustained-release DHP calcium channel blocker", "30mg", 28.00f, "Rx" },
    { "Felodipine (Plendil)", "Cardiovascular", "Hypertension, DHP CCB with vascular selectivity", "5mg", 22.00f, "Rx" },
    { "Nisoldipine (Sular)", "Cardiovascular", "Hypertension, highly vascular-selective DHP CCB", "8.5mg", 24.00f, "Rx" },
    { "Isradipine (DynaCirc)", "Cardiovascular", "Hypertension, DHP CCB with moderate vascular selectivity", "2.5mg", 22.00f, "Rx" },
    { "Clevidipine (Cleviprex-oral)", "Cardiovascular", "Hypertension emergency, IV ultra-short DHP CCB", "0.5mg/mL", 85.00f, "Rx" },
    { "Nifedipine ER (Procardia XL)", "Cardiovascular", "Hypertension and angina, DHP CCB extended-release", "30mg", 18.00f, "Rx" },
    { "Verapamil (Calan)", "Cardiovascular", "Hypertension, angina, SVT, non-DHP CCB rate control", "80mg", 12.00f, "Rx" },
    { "Diltiazem (Cardizem)", "Cardiovascular", "Hypertension, angina, AF rate control, non-DHP CCB", "120mg", 14.00f, "Rx" },
    { "Perindopril (Aceon)", "Cardiovascular", "Hypertension and CAD, ACE inhibitor stable coronary disease", "4mg", 18.00f, "Rx" },
    { "Fosinopril (Monopril)", "Cardiovascular", "Hypertension and heart failure, dual excretion ACE inhibitor", "10mg", 16.00f, "Rx" },
    { "Moexipril (Univasc)", "Cardiovascular", "Hypertension, ACE inhibitor prodrug", "7.5mg", 18.00f, "Rx" },
    { "Quinapril (Accupril)", "Cardiovascular", "Hypertension and heart failure, ACE inhibitor", "10mg", 16.00f, "Rx" },
    { "Trandolapril (Mavik)", "Cardiovascular", "Hypertension and post-MI, long-acting ACE inhibitor", "1mg", 18.00f, "Rx" },
    { "Benazepril (Lotensin)", "Cardiovascular", "Hypertension, ACE inhibitor with minimal first-dose effect", "10mg", 16.00f, "Rx" },
    { "Olmesartan (Benicar)", "Cardiovascular", "Hypertension, ARB prodrug converted to olmesartan", "20mg", 28.00f, "Rx" },
    { "Telmisartan (Micardis)", "Cardiovascular", "Hypertension and CV risk reduction, ARB PPAR-gamma activity", "40mg", 28.00f, "Rx" },
    { "Irbesartan (Avapro)", "Cardiovascular", "Hypertension and diabetic nephropathy, ARB", "150mg", 24.00f, "Rx" },
    { "Eprosartan (Teveten)", "Cardiovascular", "Hypertension, ARB with presynaptic angiotensin blockade", "400mg", 28.00f, "Rx" },
    { "Candesartan (Atacand)", "Cardiovascular", "Hypertension and heart failure, high-affinity ARB", "8mg", 28.00f, "Rx" },
    { "Nebivolol (Bystolic-alt)", "Cardiovascular", "Hypertension, beta-1 selective blocker with NO-mediated vasodilation", "5mg", 35.00f, "Rx" },
    { "Carvedilol (Coreg)", "Cardiovascular", "Heart failure and hypertension, non-selective beta plus alpha-1 blocker", "6.25mg", 12.00f, "Rx" },
    { "Bisoprolol (Zebeta)", "Cardiovascular", "Hypertension and heart failure, highly beta-1 selective blocker", "5mg", 14.00f, "Rx" },
    { "Atenolol (Tenormin)", "Cardiovascular", "Hypertension and angina, beta-1 selective blocker", "50mg", 10.00f, "Rx" },
    { "Acebutolol (Sectral)", "Cardiovascular", "Hypertension and arrhythmia, ISA beta-1 selective blocker", "200mg", 16.00f, "Rx" },
    { "Pindolol (Visken)", "Cardiovascular", "Hypertension, non-selective beta-blocker with ISA", "5mg", 14.00f, "Rx" },
    { "Penbutolol (Levatol)", "Cardiovascular", "Hypertension, non-selective beta-blocker with ISA", "20mg", 16.00f, "Rx" },
    { "Propranolol LA (Inderal LA)", "Cardiovascular", "Hypertension, angina, tremor, migraine prevention, non-selective BB", "60mg", 14.00f, "Rx" },
    { "Timolol (Blocadren)", "Cardiovascular", "Hypertension, angina, post-MI, non-selective beta-blocker", "10mg", 12.00f, "Rx" },
    { "Sotalol AF (Sorine)", "Cardiovascular", "AF maintenance, class II/III pure sotalol indication", "80mg", 18.00f, "Rx" },
    { "Digoxin (Lanoxin)", "Cardiovascular", "AF rate control and heart failure, cardiac glycoside", "0.125mg", 8.00f, "Rx" },
    { "Atropine IV", "Emergency", "Bradycardia, anticholinergic muscarinic antagonist IV", "0.5mg IV", 8.00f, "Rx" },
    { "Isoproterenol (Isuprel)", "Emergency", "Refractory bradycardia and AV block, non-selective beta agonist", "0.02mcg/kg/min IV", 28.00f, "Rx" },
    { "Nitroprusside/Nitric Oxide (iNO)", "Emergency", "Persistent pulmonary hypertension neonates, inhaled vasodilator", "20ppm inhaled", 480.00f, "Rx" },
    { "Albumin (Buminate)", "Emergency", "Hypoalbuminemia, volume replacement, plasma protein product", "25g IV", 85.00f, "Rx" },
    { "Dextran 40 (Rheomacrodex)", "Emergency", "Thromboembolism prophylaxis and volume expansion, polysaccharide", "10% 500mL IV", 45.00f, "Rx" },
    { "Hetastarch (Hespan)", "Emergency", "Volume expansion, synthetic colloid hydroxyethyl starch", "6% 500mL IV", 38.00f, "Rx" },
    { "Lactated Ringer's Solution", "Emergency", "Volume resuscitation and maintenance IV crystalloid fluid", "500mL IV", 5.00f, "Rx" },
    { "Normal Saline 0.9%", "Emergency", "Volume resuscitation and medication dilution IV crystalloid", "1000mL IV", 4.00f, "Rx" },
    { "Hypertonic Saline 3%", "Emergency", "Hyponatremia correction and cerebral edema, hypertonic IV", "100mL IV", 18.00f, "Rx" },
    { "Dextrose 50% (D50)", "Emergency", "Hypoglycemia emergency, concentrated glucose IV push", "25g IV", 6.00f, "Rx" },
    { "Calcium Gluconate IV", "Emergency", "Hypocalcemia, hyperkalemia, CCB toxicity, IV calcium", "1g IV", 8.00f, "Rx" },
    { "Magnesium Sulfate IV", "Emergency", "Eclampsia, torsades de pointes, hypomagnesemia IV", "2g IV", 8.00f, "Rx" },
    { "Sodium Bicarbonate IV", "Emergency", "Metabolic acidosis and TCA toxicity, alkalinizing agent IV", "50mEq IV", 6.00f, "Rx" },
    { "Potassium Chloride IV (KCl)", "Emergency", "Hypokalemia replacement, IV potassium supplementation", "10mEq IV", 8.00f, "Rx" },
    { "Phosphate IV (Sodium Phosphate)", "Emergency", "Severe hypophosphatemia, IV phosphorus replacement", "15mmol IV", 12.00f, "Rx" },
    { "Dexamethasone (DexAM)", "Emergency", "Airway edema, meningitis, croup, potent anti-inflammatory", "0.6mg/kg", 8.00f, "Rx" },
    { "Naloxone (Narcan)", "Emergency", "Opioid overdose reversal, mu-opioid receptor antagonist", "0.4mg IV/IN", 18.00f, "OTC" },
    { "Flumazenil (Romazicon)", "Emergency", "Benzodiazepine overdose reversal, GABA-A antagonist IV", "0.2mg IV", 28.00f, "Rx" },
    { "Physostigmine (Antilirium)", "Emergency", "Anticholinergic toxicity, reversible AChE inhibitor crosses BBB", "1mg IV", 22.00f, "Rx" },
    { "Pralidoxime (2-PAM)", "Emergency", "Organophosphate poisoning, acetylcholinesterase reactivator", "1g IV", 85.00f, "Rx" },
    { "Atropine/Pralidoxime (DuoDote)", "Emergency", "Nerve agent and organophosphate exposure, autoinjector", "2.1/600mg IM", 95.00f, "Rx" },
    { "Cyanide Antidote Kit (Nithiodote)", "Emergency", "Cyanide poisoning, sodium nitrite plus sodium thiosulfate", "300mg IV", 180.00f, "Rx" },
    { "Hydroxocobalamin (Cyanokit)", "Emergency", "Cyanide poisoning, scavenges HCN cofactor antidote", "5g IV", 480.00f, "Rx" },
    { "Methylene Blue (Urolene Blue)", "Emergency", "Methemoglobinemia treatment, reducing agent restores Fe2+", "1mg/kg IV", 45.00f, "Rx" },
    { "Fomepizole (Antizol)", "Emergency", "Methanol and ethylene glycol poisoning, alcohol dehydrogenase blocker", "15mg/kg IV", 850.00f, "Rx" },
    { "Digoxin-Specific Antibody (Digibind)", "Emergency", "Digoxin toxicity reversal, Fab antibody fragments", "40mg IV", 1200.00f, "Rx" },
    { "Deferoxamine (Desferal)", "Emergency", "Iron poisoning and thalassemia, iron chelator IV/SC", "1g IV", 145.00f, "Rx" },
    { "Deferasirox (Exjade)", "Blood", "Chronic iron overload transfusion chelation, oral tridentate chelator", "500mg", 85.00f, "Rx" },
    { "Deferiprone (Ferriprox)", "Blood", "Thalassemia iron overload, oral bidentate iron chelator", "500mg", 95.00f, "Rx" },
    { "Succimer (Chemet)", "Emergency", "Lead poisoning in children, oral DMSA lead chelator", "10mg/kg", 38.00f, "Rx" },
    { "Dimercaprol (BAL in Oil)", "Emergency", "Heavy metal poisoning arsenic/mercury/lead, dithiol chelator IM", "3mg/kg IM", 85.00f, "Rx" },
    { "Edetate Calcium Disodium (Calcium EDTA)", "Emergency", "Lead poisoning, calcium EDTA IV chelator therapy", "1g/m2 IV", 45.00f, "Rx" },
    { "Penicillamine (Cuprimine-toxicity)", "Emergency", "Copper/mercury/lead chelation and Wilson disease treatment", "250mg", 22.00f, "Rx" },
    { "Trientine (Syprine)", "Emergency", "Wilson disease, copper chelator second-line therapy", "250mg", 85.00f, "Rx" },
    { "Zinc Acetate (Galzin)", "Digestive", "Wilson disease maintenance, intestinal copper absorption blocker", "50mg", 45.00f, "Rx" },
    { "Miglustat (Zavesca)", "Neurological", "Gaucher type 1 and NPC, glucosylceramide synthase inhibitor", "100mg", 4800.00f, "Rx" },
    { "Imiglucerase (Cerezyme)", "Blood", "Gaucher disease type 1, recombinant glucocerebrosidase ERT", "400units IV", 12000.00f, "Rx" },
    { "Velaglucerase (VPRIV)", "Blood", "Gaucher disease, gene-activated glucocerebrosidase ERT", "400units IV", 12500.00f, "Rx" },
    { "Taliglucerase Alfa (Elelyso)", "Blood", "Gaucher disease, plant-cell produced glucocerebrosidase ERT", "60units/kg IV", 11000.00f, "Rx" },
    { "Eliglustat (Cerdelga)", "Blood", "Gaucher type 1, oral glucosylceramide synthase inhibitor", "84mg", 8500.00f, "Rx" },
    { "Agalsidase Beta (Fabrazyme)", "Blood", "Fabry disease, recombinant alpha-galactosidase A ERT", "1mg/kg IV", 18000.00f, "Rx" },
    { "Migalastat (Galafold)", "Blood", "Fabry disease amenable mutations, oral pharmacological chaperone", "123mg", 14000.00f, "Rx" },
    { "Alglucosidase Alfa (Myozyme)", "Blood", "Pompe disease, recombinant acid alpha-glucosidase ERT", "20mg/kg IV", 22000.00f, "Rx" },
    { "Avalglucosidase Alfa (Nexviazyme)", "Blood", "Pompe disease, next-gen acid alpha-glucosidase higher uptake ERT", "20mg/kg IV", 24000.00f, "Rx" },
    { "Cipaglucosidase/Miglustat (Pombiliti)", "Blood", "Late-onset Pompe, ERT with pharmacological chaperone combination", "20mg/kg IV", 25000.00f, "Rx" },
    { "Sebelipase Alfa (Kanuma)", "Blood", "Lysosomal acid lipase deficiency, recombinant LAL enzyme", "1mg/kg IV", 28000.00f, "Rx" },
    { "Laronidase (Aldurazyme)", "Blood", "MPS I Hurler/Scheie, recombinant alpha-L-iduronidase ERT", "0.58mg/kg IV", 16000.00f, "Rx" },
    { "Idursulfase (Elaprase)", "Blood", "MPS II Hunter syndrome, recombinant iduronate-2-sulfatase ERT", "0.5mg/kg IV", 18000.00f, "Rx" },
    { "Elosulfase Alfa (Vimizim)", "Blood", "MPS IVA Morquio A, N-acetylgalactosamine-6-sulfatase ERT", "2mg/kg IV", 20000.00f, "Rx" },
    { "Galsulfase (Naglazyme)", "Blood", "MPS VI Maroteaux-Lamy, N-acetylgalactosamine-4-sulfatase ERT", "1mg/kg IV", 19000.00f, "Rx" },
    { "Vestronidase Alfa (Mepsevii)", "Blood", "MPS VII Sly syndrome, beta-glucuronidase ERT", "4mg/kg IV", 21000.00f, "Rx" },
    { "Cerliponase Alfa (Brineura)", "Neurological", "Batten disease CLN2, tripeptidyl peptidase 1 intraventricular", "300mg ICV", 158000.00f, "Rx" },
    { "Olipudase Alfa (Xenpozyme)", "Blood", "Acid sphingomyelinase deficiency, recombinant ASM ERT", "3mg/kg IV", 28000.00f, "Rx" },
    { "Crizanlizumab (Adakveo)", "Blood", "Sickle cell disease crisis prevention, anti-P-selectin antibody", "5mg/kg IV", 2800.00f, "Rx" },
    { "L-Glutamine (Endari)", "Blood", "Sickle cell disease crisis reduction, amino acid reduces oxidative stress", "15g sachet", 280.00f, "Rx" },
    { "Voxelotor (Oxbryta)", "Blood", "Sickle cell disease, hemoglobin S polymerization inhibitor", "1500mg", 1800.00f, "Rx" },
    { "Hydroxyurea (Droxia)", "Blood", "Sickle cell disease and CML, ribonucleotide reductase inhibitor", "500mg", 18.00f, "Rx" },
    { "Luspatercept (Reblozyl-SCD)", "Blood", "Beta-thalassemia and MDS, erythroid maturation agent TGF-beta", "1mg/kg SC", 3200.00f, "Rx" },
    { "Betibeglogene Autotemcel (Zynteglo)", "Blood", "Beta-thalassemia, lentiviral gene therapy HbAT87Q", "single dose IV", 2800000.00f, "Rx" },
    { "Lovotibeglogene Autotemcel (Lyfgenia)", "Blood", "Sickle cell disease, lentiviral gene therapy HbAT87Q-anti-sickling", "single dose IV", 3100000.00f, "Rx" },
    { "Exagamglogene Autotemcel (Casgevy)", "Blood", "SCD and beta-thal, first CRISPR/Cas9 approved gene edit therapy", "single dose IV", 2200000.00f, "Rx" },
    { "Filgrastim (Neupogen)", "Blood", "Neutropenia, G-CSF granulocyte colony stimulating factor", "300mcg SC", 280.00f, "Rx" },
    { "Pegfilgrastim (Neulasta)", "Blood", "Chemotherapy neutropenia prophylaxis, pegylated G-CSF", "6mg SC", 380.00f, "Rx" },
    { "Sargramostim (Leukine)", "Blood", "Stem cell transplant support, GM-CSF granulocyte-macrophage CSF", "250mcg SC", 480.00f, "Rx" },
    { "Eltrombopag (Promacta)", "Blood", "ITP and AA, oral thrombopoietin receptor agonist", "50mg", 450.00f, "Rx" },
    { "Romiplostim (Nplate)", "Blood", "ITP, peptibody TPO receptor agonist SC weekly", "1mcg/kg SC", 380.00f, "Rx" },
    { "Lusutrombopag (Mulpleta)", "Blood", "Thrombocytopenia chronic liver disease, oral TPO-RA", "3mg", 380.00f, "Rx" },
    { "Fostamatinib (Tavalisse)", "Blood", "ITP, oral spleen tyrosine kinase SYK inhibitor", "100mg", 450.00f, "Rx" },
    { "Rilzabrutinib", "Blood", "ITP, oral reversible BTK inhibitor in development", "400mg", 0.00f, "Rx" },
    { "Efgartigimod Alfa (Argenx-ITP)", "Blood", "ITP, FcRn inhibitor reduces pathogenic IgG autoantibodies", "10mg/kg IV", 3200.00f, "Rx" },
    { "Ibuprofen Topical (Voltaren-alt)", "Pain Relief", "Localized joint and muscle pain, topical NSAID gel", "5%", 14.99f, "OTC" },
    { "Diclofenac Topical (Voltaren Gel)", "Pain Relief", "Osteoarthritis joint pain, topical NSAID anti-inflammatory", "1%", 14.99f, "OTC" },
    { "Diclofenac Patch (Flector)", "Pain Relief", "Acute pain sprains strains, topical NSAID patch", "1.3%", 28.00f, "Rx" },
    { "Celecoxib/Pregabalin (theoretical)", "Pain Relief", "Multimodal pain combination theoretical", "200/75mg", 0.00f, "Rx" },
    { "Cyclobenzaprine (Flexeril)", "Pain Relief", "Muscle spasm, centrally acting skeletal muscle relaxant TCA-rel", "10mg", 12.00f, "Rx" },
    { "Tizanidine (Zanaflex)", "Pain Relief", "Muscle spasticity, central alpha-2 agonist imidazoline", "4mg", 15.00f, "Rx" },
    { "Carisoprodol (Soma)", "Pain Relief", "Muscle spasm, centrally acting sedative muscle relaxant", "350mg", 18.00f, "Rx" },
    { "Chlorzoxazone (Paraflex)", "Pain Relief", "Musculoskeletal pain spasm, centrally acting muscle relaxant", "500mg", 14.00f, "Rx" },
    { "Metaxalone (Skelaxin)", "Pain Relief", "Musculoskeletal pain, centrally acting muscle relaxant", "800mg", 22.00f, "Rx" },
    { "Baclofen (Lioresal)", "Neurological", "Spasticity MS and spinal cord, GABA-B receptor agonist", "10mg", 12.00f, "Rx" },
    { "Dantrolene (Dantrium)", "Emergency", "Malignant hyperthermia and spasticity, ryanodine receptor blocker", "25mg", 28.00f, "Rx" },
    { "Orphenadrine (Norflex)", "Pain Relief", "Musculoskeletal pain, anticholinergic muscle relaxant analogue", "100mg ER", 16.00f, "Rx" },
    { "Colchicine (Colcrys)", "Pain Relief", "Gout acute flare and familial Mediterranean fever, microtubule inhibitor", "0.6mg", 18.00f, "Rx" },
    { "Febuxostat (Uloric)", "Pain Relief", "Gout prevention, non-purine XO inhibitor urate lowering", "40mg", 28.00f, "Rx" },
    { "Probenecid", "Pain Relief", "Gout, uricosuric URAT1 inhibitor increases uric acid excretion", "500mg", 12.00f, "Rx" },
    { "Benzbromarone", "Pain Relief", "Gout, potent uricosuric URAT1 inhibitor Europe/Asia use", "100mg", 22.00f, "Rx" },
    { "Pegloticase (Krystexxa)", "Pain Relief", "Chronic refractory gout, PEGylated recombinant uricase", "8mg IV", 4800.00f, "Rx" },
    { "Canakinumab (Ilaris-gout)", "Pain Relief", "Recurrent gout flares, IL-1beta antibody anti-inflammatory", "150mg SC", 5200.00f, "Rx" },
    { "Lanthanum Carbonate (Fosrenol)", "Digestive", "Hyperphosphatemia in ESRD, non-calcium phosphate binder", "500mg", 38.00f, "Rx" },
    { "Sevelamer Carbonate (Renvela)", "Digestive", "Hyperphosphatemia in CKD, non-absorbable phosphate binder", "800mg", 35.00f, "Rx" },
    { "Calcium Acetate (PhosLo)", "Digestive", "Hyperphosphatemia, phosphate binder with calcium supplement", "667mg", 18.00f, "Rx" },
    { "Ferric Citrate (Auryxia)", "Digestive", "Hyperphosphatemia and iron deficiency in CKD, iron-based binder", "210mg", 28.00f, "Rx" },
    { "Sucroferric Oxyhydroxide (Velphoro)", "Digestive", "Hyperphosphatemia CKD, chewable iron-based phosphate binder", "500mg", 32.00f, "Rx" },
    { "Patiromer (Veltassa)", "Digestive", "Hyperkalemia, oral potassium-binding polymer non-absorbable", "8.4g sachet", 85.00f, "Rx" },
    { "Sodium Zirconium Cyclosilicate (Lokelma)", "Digestive", "Hyperkalemia, ion-exchange zirconium sodium potassium binder", "10g sachet", 95.00f, "Rx" },
    { "Rifaximin (Xifaxan)", "Digestive", "Traveler's diarrhea, HE, and IBS-D, non-absorbable rifamycin", "200mg", 18.00f, "Rx" },
    { "Fidaxomicin (Dificid)", "Digestive", "C. difficile infection, non-systemic macrocyclic antibiotic", "200mg", 145.00f, "Rx" },
    { "Vancomycin Oral (Vancocin PO)", "Digestive", "C. difficile colitis, locally active glycopeptide bowel treatment", "125mg", 28.00f, "Rx" },
    { "Bezlotoxumab (Zinplava)", "Digestive", "C. difficile recurrence prevention, anti-toxin B antibody", "10mg/kg IV", 4200.00f, "Rx" },
    { "Linaclotide (Linzess)", "Digestive", "IBS-C and chronic constipation, guanylate cyclase-C agonist", "145mcg", 85.00f, "Rx" },
    { "Plecanatide (Trulance)", "Digestive", "IBS-C and chronic idiopathic constipation, GC-C agonist", "3mg", 88.00f, "Rx" },
    { "Prucalopride (Motegrity)", "Digestive", "Chronic idiopathic constipation, selective 5-HT4 receptor agonist", "2mg", 85.00f, "Rx" },
    { "Tegaserod (Zelnorm)", "Digestive", "IBS-C women <65, partial 5-HT4 agonist prokinetic", "6mg", 45.00f, "Rx" },
    { "Tenapanor (Ibsrela)", "Digestive", "IBS-C, intestinal NHE3 sodium/hydrogen exchanger 3 inhibitor", "50mg", 95.00f, "Rx" },
    { "Methylnaltrexone (Relistor)", "Digestive", "Opioid-induced constipation, peripheral mu antagonist SC", "12mg SC", 45.00f, "Rx" },
    { "Naloxegol (Movantik)", "Digestive", "Opioid-induced constipation, PEGylated peripheral mu antagonist", "25mg", 28.00f, "Rx" },
    { "Alvimopan (Entereg)", "Digestive", "Post-op ileus, peripheral mu-opioid receptor antagonist", "12mg", 35.00f, "Rx" },
    { "Alosetron (Lotronex)", "Digestive", "Severe IBS-D women, 5-HT3 antagonist limited REMS", "0.5mg", 28.00f, "Rx" },
    { "Ondansetron (Zofran)", "Digestive", "Chemotherapy and surgery nausea, 5-HT3 receptor antagonist", "4mg", 8.00f, "Rx" },
    { "Granisetron (Kytril)", "Digestive", "Chemotherapy nausea prevention, 5-HT3 receptor antagonist", "1mg", 12.00f, "Rx" },
    { "Dolasetron (Anzemet)", "Digestive", "Chemotherapy-induced nausea and vomiting, 5-HT3 antagonist", "100mg", 18.00f, "Rx" },
    { "Palonosetron (Aloxi)", "Digestive", "CINV delayed phase, long-acting 5-HT3 receptor antagonist", "0.25mg IV", 85.00f, "Rx" },
    { "Netupitant/Palonosetron (Akynzeo)", "Digestive", "CINV, NK1 receptor antagonist plus 5-HT3 antagonist combo", "300/0.5mg", 145.00f, "Rx" },
    { "Rolapitant (Varubi)", "Digestive", "CINV delayed nausea, NK1 receptor antagonist", "180mg", 95.00f, "Rx" },
    { "Prochlorperazine (Compazine)", "Digestive", "Nausea, vomiting, anxiety, dopamine D2 antagonist phenothiazine", "10mg", 8.00f, "Rx" },
    { "Trimethobenzamide (Tigan)", "Digestive", "Nausea and vomiting, antiemetic D2 antagonist IM/oral", "300mg", 18.00f, "Rx" },
    { "Metoclopramide (Reglan)", "Digestive", "GERD, gastroparesis, nausea, dopamine antagonist prokinetic", "10mg", 8.00f, "Rx" },
    { "Domperidone (Motilium)", "Digestive", "Gastroparesis, peripheral dopamine D2 antagonist prokinetic", "10mg", 12.00f, "Rx" },
    { "Droperidol (Inapsine)", "Emergency", "Perioperative nausea and sedation, butyrophenone D2 antagonist", "1.25mg IV", 8.00f, "Rx" },
    { "Erythromycin (GI Motility)", "Digestive", "Gastroparesis, motilin receptor agonist prokinetic low dose", "250mg", 14.00f, "Rx" },
    { "Neostigmine (Prostigmin)", "Digestive", "Acute colonic pseudo-obstruction, AChE inhibitor prokinetic", "2.5mg IV", 12.00f, "Rx" },
    { "Lubiprostone (Amitiza)", "Digestive", "Chronic constipation and IBS-C, chloride channel activator CIC-2", "8mcg", 28.00f, "Rx" },
    { "Bisacodyl (Dulcolax)", "Digestive", "Constipation, stimulant laxative osmotic irritant action", "5mg", 4.99f, "OTC" },
    { "Senna (Senokot)", "Digestive", "Constipation, anthraquinone stimulant laxative", "8.6mg", 5.99f, "OTC" },
    { "Polyethylene Glycol (MiraLAX)", "Digestive", "Constipation and bowel prep, osmotic laxative", "17g sachet", 7.99f, "OTC" },
    { "Lactulose (Enulose)", "Digestive", "Constipation and hepatic encephalopathy, osmotic laxative", "10g/15mL", 12.00f, "Rx" },
    { "Sorbitol 70%", "Digestive", "Constipation and drug overdose sorption, osmotic agent", "30mL", 8.00f, "OTC" },
    { "Magnesium Hydroxide (Milk of Magnesia)", "Digestive", "Constipation and antacid, osmotic laxative Mg2+ saline", "400mg/5mL", 5.99f, "OTC" },
    { "Magnesium Citrate", "Digestive", "Bowel prep and constipation, osmotic saline laxative", "240mL", 6.99f, "OTC" },
    { "Docusate Sodium (Colace)", "Digestive", "Constipation stool softener, anionic surfactant emollient", "100mg", 5.99f, "OTC" },
    { "Glycerin Suppository", "Digestive", "Constipation, osmotic and lubricant rectal suppository", "2.1g", 4.99f, "OTC" },
    { "Psyllium (Metamucil)", "Digestive", "Constipation and cholesterol, soluble fiber bulk-forming laxative", "3.4g dose", 12.99f, "OTC" },
    { "Methylcellulose (Citrucel)", "Digestive", "Constipation, non-fermentable bulk-forming fiber laxative", "2g dose", 10.99f, "OTC" },
    { "Simethicone (Gas-X)", "Digestive", "Gas and bloating, anti-foam agent coalescence of bubbles", "125mg", 6.99f, "OTC" },
    { "Activated Charcoal (Actidose)", "Emergency", "Drug overdose adsorption, non-specific toxin binder", "50g", 18.00f, "OTC" },
    { "Ipecac Syrup (historical)", "Emergency", "Emetic for poisoning, historical use discouraged now", "15mL", 5.00f, "OTC" },
    { "Zinc Oxide (Desitin)", "Skin", "Diaper rash and skin barrier, topical astringent protectant", "40%", 8.99f, "OTC" },
    { "Petrolatum (Aquaphor)", "Skin", "Dry skin and wound protection, occlusive emollient barrier", "100%", 10.99f, "OTC" },
    { "Urea Cream (Uremol)", "Skin", "Dry skin and keratosis, keratolytic and moisturizing", "20%", 12.99f, "OTC" },
    { "Salicylic Acid (Compound W)", "Skin", "Warts and acne, keratolytic exfoliant beta-hydroxy acid", "17%", 8.99f, "OTC" },
    { "Podophyllotoxin (Condylox)", "Skin", "Genital warts, antimitotic topical agent podophyllin extract", "0.5%", 28.00f, "Rx" },
    { "Imiquimod (Aldara)", "Skin", "Genital warts and AK, TLR7 agonist immune response modifier", "5%", 28.00f, "Rx" },
    { "Sinecatechins (Veregen)", "Skin", "External genital warts, green tea catechin botanical extract", "15%", 38.00f, "Rx" },
    { "5-Fluorouracil Topical (Efudex)", "Skin", "Actinic keratosis and SCC in situ, topical antimetabolite", "5%", 28.00f, "Rx" },
    { "Diclofenac Gel (Solaraze)", "Skin", "Actinic keratosis, topical NSAID anti-inflammatory", "3%", 32.00f, "Rx" },
    { "Ingenol Mebutate (Picato)", "Skin", "Actinic keratosis, euphorbia plant derivative PKC activator", "0.015%", 45.00f, "Rx" },
    { "Aminolevulinic Acid (Levulan)", "Skin", "Actinic keratosis PDT photosensitizer, porphyrin precursor", "20%", 95.00f, "Rx" },
    { "Tirbanibulin (Klisyri)", "Skin", "Actinic keratosis, tubulin and Src kinase inhibitor topical", "1%", 52.00f, "Rx" },
    { "Fluorouracil/Salicylic Acid (Tolak)", "Skin", "Actinic keratosis, combination antimetabolite/keratolytic", "4%/17%", 38.00f, "Rx" },
    { "Mupirocin (Bactroban)", "Skin", "Impetigo and MRSA decolonization, bacterial IRNA synthetase inhibitor", "2%", 18.00f, "Rx" },
    { "Retapamulin (Altabax)", "Skin", "Impetigo, topical pleuromutilin bacterial protein synthesis inhibitor", "1%", 22.00f, "Rx" },
    { "Ozenoxacin (Xepi)", "Skin", "Impetigo, topical non-fluorinated quinolone antibacterial", "1%", 28.00f, "Rx" },
    { "Omadacycline (Nuzyra)", "Antibiotic", "CAP and ABSSSI, aminomethylcycline tetracycline class IV/PO", "300mg", 85.00f, "Rx" },
    { "Eravacycline (Xerava)", "Antibiotic", "Complicated IAI, fully synthetic fluorocycline IV", "1mg/kg IV", 180.00f, "Rx" },
    { "Cefiderocol (Fetroja)", "Antibiotic", "Gram-negative MDR including carbapenem-resistant, siderophore cephalosporin", "2g IV", 480.00f, "Rx" },
    { "Imipenem/Cilastatin/Relebactam (Recarbrio)", "Antibiotic", "Carbapenem-resistant gram-negatives, imipenem plus new beta-lactamase inh", "500mg IV", 380.00f, "Rx" },
    { "Meropenem/Vaborbactam (Vabomere)", "Antibiotic", "KPC-producing Klebsiella UTI/BSI, carbapenem plus cyclic boronate BLI", "2g/2g IV", 450.00f, "Rx" },
    { "Ceftazidime/Avibactam (Avycaz)", "Antibiotic", "MDR gram-negative and KPC, cephalosporin plus non-beta-lactam BLI", "2.5g IV", 480.00f, "Rx" },
    { "Ceftolozane/Tazobactam (Zerbaxa)", "Antibiotic", "MDR Pseudomonas and complicated UTI/IAI, anti-Pseudomonal cephalosporin", "1.5g IV", 420.00f, "Rx" },
    { "Aztreonam/Avibactam (Emblaveo)", "Antibiotic", "MBL-producing gram-negatives, monobactam plus new BLI combination", "6.5g IV", 580.00f, "Rx" },
    { "Cefepime/Enmetazobactam (Exblifep)", "Antibiotic", "ESBL UTI complicated, cephalosporin plus penicillin-binding BLI", "2.5g IV", 380.00f, "Rx" },
    { "Delafloxacin (Baxdela)", "Antibiotic", "ABSSSI and CAP, anionic fluoroquinolone active low pH", "450mg", 85.00f, "Rx" },
    { "Lefamulin (Xenleta)", "Antibiotic", "CAP, first pleuromutilin IV/oral for systemic infection", "150mg IV", 145.00f, "Rx" },
    { "Cefazolin/Sulbactam", "Antibiotic", "Acinetobacter and beta-lactamase infections, combination", "3g IV", 95.00f, "Rx" },
    { "Dalbavancin (Dalvance)", "Antibiotic", "ABSSSI, long-acting lipoglycopeptide once or twice weekly IV", "1500mg IV", 850.00f, "Rx" },
    { "Oritavancin (Orbactiv)", "Antibiotic", "ABSSSI, single-dose long-acting lipoglycopeptide IV", "1200mg IV", 1200.00f, "Rx" },
    { "Telavancin (Vibativ)", "Antibiotic", "HABP/VABP and ABSSSI, lipoglycopeptide TV once daily IV", "10mg/kg IV", 480.00f, "Rx" },
    { "Daptomycin (Cubicin)", "Antibiotic", "MRSA BSI, endocarditis, ABSSSI, cyclic lipopeptide IV", "4mg/kg IV", 245.00f, "Rx" },
    { "Tigecycline (Tygacil)", "Antibiotic", "MDR infections and complicated IAI/SSTI, glycylcycline IV", "50mg IV", 185.00f, "Rx" },
    { "Polymyxin B (Poly-Rx)", "Antibiotic", "MDR gram-negatives, polypeptide outer membrane disruptor IV", "2.5mg/kg IV", 145.00f, "Rx" },
    { "Colistin/Colistimethate (Coly-Mycin)", "Antibiotic", "MDR gram-negatives, polymyxin E prodrug IV/inhaled", "5mg/kg IV", 155.00f, "Rx" },
    { "Fosfomycin IV (Fomicyt)", "Antibiotic", "Complicated UTI and MDR gram-negatives, IV phosphonic acid", "4g IV", 85.00f, "Rx" },
    { "Tedizolid (Sivextro-alt)", "Antibiotic", "ABSSSI shorter 6-day course, next-gen oxazolidinone", "200mg", 310.00f, "Rx" },
    { "Quinupristin/Dalfopristin (Synercid)", "Antibiotic", "VRE faecium and MRSA, streptogramin combination IV", "7.5mg/kg IV", 380.00f, "Rx" },
    { "Chloramphenicol", "Antibiotic", "Meningitis, typhoid, serious infections broad-spectrum, protein syn", "500mg", 18.00f, "Rx" },
    { "Fusidic Acid (Fucidin)", "Antibiotic", "Staphylococcal skin infections, steroid acid antibiotic topical/oral", "250mg", 22.00f, "Rx" },
    { "Fidaxomicin (Dificid-alt)", "Antibiotic", "C. difficile, macrocyclic antibiotic narrow spectrum", "200mg", 145.00f, "Rx" },
    { "Delamanid (Deltyba)", "Antibiotic", "MDR tuberculosis, bicyclic nitroimidazole anti-TB", "50mg", 485.00f, "Rx" },
    { "Bedaquiline (Sirturo)", "Antibiotic", "MDR tuberculosis, ATP synthase inhibitor diarylquinoline", "400mg", 380.00f, "Rx" },
    { "Pretomanid (PA-824)", "Antibiotic", "XDR/pre-XDR tuberculosis with BPaL regimen, nitroimidazole", "200mg", 340.00f, "Rx" },
    { "Isoniazid (INH)", "Antibiotic", "Tuberculosis treatment and prophylaxis, InhA inhibitor prodrug", "300mg", 8.00f, "Rx" },
    { "Pyrazinamide (PZA)", "Antibiotic", "TB treatment first 2 months, nicotinamide analogue bactericidal", "1500mg", 12.00f, "Rx" },
    { "Ethambutol (Myambutol)", "Antibiotic", "Tuberculosis combination therapy, arabinosyl transferase inhibitor", "800mg", 15.00f, "Rx" },
    { "Streptomycin (TB)", "Antibiotic", "TB combination therapy, aminoglycoside first anti-TB drug", "1g IM", 18.00f, "Rx" },
    { "Amikacin (TB use)", "Antibiotic", "MDR-TB injectable aminoglycoside second-line agent", "15mg/kg IV", 68.00f, "Rx" },
    { "Capreomycin (Capastat)", "Antibiotic", "MDR-TB, cyclic peptide injectable second-line agent", "1g IM", 45.00f, "Rx" },
    { "Cycloserine (Seromycin)", "Antibiotic", "MDR-TB, d-alanine analogue bacteriostatic second-line TB", "250mg", 38.00f, "Rx" },
    { "Ethionamide (Trecator)", "Antibiotic", "MDR-TB, nicotinamide analogue InhA inhibitor second-line", "250mg", 28.00f, "Rx" },
    { "Para-Aminosalicylic Acid (PAS)", "Antibiotic", "MDR-TB, folate competitor bacteriostatic TB second-line", "4g sachet", 45.00f, "Rx" },
    { "Clofazimine (Lamprene)", "Antibiotic", "Leprosy and MDR-TB, riminophenazine anti-mycobacterial", "100mg", 38.00f, "Rx" },
    { "Dapsone (Aczone-systemic)", "Antibiotic", "Leprosy, PCP prophylaxis, dermatitis herpetiformis, sulfone", "100mg", 18.00f, "Rx" },
    { "Albendazole (Albenza)", "Antibiotic", "Neurocysticercosis, hydatid disease, GI nematodes, benzimidazole", "400mg", 18.00f, "Rx" },
    { "Mebendazole (Vermox)", "Antibiotic", "Pinworms and roundworms, benzimidazole tubulin inhibitor", "100mg", 12.00f, "OTC" },
    { "Pyrantel Pamoate (Pin-X)", "Antibiotic", "Pinworm and roundworm, depolarizing neuromuscular blocker", "62.5mg/mL", 8.99f, "OTC" },
    { "Praziquantel (Biltricide)", "Antibiotic", "Schistosomiasis and tapeworms, calcium channel activator", "600mg", 28.00f, "Rx" },
    { "Ivermectin (Stromectol)", "Antibiotic", "River blindness, strongyloidiasis, scabies, glutamate-gated Cl channel", "3mg", 18.00f, "Rx" },
    { "Diethylcarbamazine (DEC)", "Antibiotic", "Lymphatic filariasis and loiasis, microfilariae immobilizer", "100mg", 22.00f, "Rx" },
    { "Suramin", "Antibiotic", "African sleeping sickness T. brucei gambiense HAT, polysulfonate", "1g IV", 180.00f, "Rx" },
    { "Pentamidine (Pentam)", "Antibiotic", "PCP treatment/prophylaxis and leishmaniasis, aromatic diamidine", "4mg/kg IV", 145.00f, "Rx" },
    { "Amphotericin B Deoxycholate (Fungizone)", "Antibiotic", "Invasive fungal infections, conventional polyene ergosterol binder", "1mg/kg IV", 145.00f, "Rx" },
    { "Liposomal Amphotericin B (AmBisome)", "Antibiotic", "Invasive fungal and leishmaniasis, liposomal amphotericin B", "3mg/kg IV", 480.00f, "Rx" },
    { "Amphotericin B Lipid Complex (Abelcet)", "Antibiotic", "Invasive fungal infections reduced nephrotoxicity lipid complex", "5mg/kg IV", 380.00f, "Rx" },
    { "Caspofungin (Cancidas)", "Antibiotic", "Invasive Aspergillus and Candida, echinocandin glucan synthase", "70mg IV", 480.00f, "Rx" },
    { "Micafungin (Mycamine)", "Antibiotic", "Invasive candidiasis and esophageal, echinocandin antifungal", "100mg IV", 380.00f, "Rx" },
    { "Anidulafungin (Eraxis)", "Antibiotic", "Invasive candidiasis, echinocandin semi-synthetic lipopeptide", "200mg IV", 350.00f, "Rx" },
    { "Rezafungin (Rezzayo)", "Antibiotic", "Candidemia and invasive candidiasis, next-gen echinocandin weekly", "400mg IV", 580.00f, "Rx" },
    { "Itraconazole (Sporanox)", "Antibiotic", "Aspergillosis, histoplasmosis, onychomycosis, triazole antifungal", "100mg", 45.00f, "Rx" },
    { "Voriconazole (Vfend)", "Antibiotic", "Invasive Aspergillosis and Candida, broad-spectrum triazole", "200mg", 85.00f, "Rx" },
    { "Posaconazole (Noxafil)", "Antibiotic", "Prophylaxis and resistant fungi, extended-spectrum triazole", "100mg", 95.00f, "Rx" },
    { "Isavuconazole (Cresemba)", "Antibiotic", "Invasive Aspergillus and Mucor, broad triazole less QTc", "186mg IV", 380.00f, "Rx" },
    { "Olorofim (Mino-Lok context)", "Antibiotic", "Invasive Aspergillus novel orotomides DHODH inhibitor oral", "120mg", 480.00f, "Rx" },
    { "Ibrexafungerp (Brexafemme)", "Antibiotic", "Vaginal candidiasis, triterpenoid glucan synthase inhibitor oral", "300mg", 150.00f, "Rx" },
    { "Oteseconazole (Vivjoa)", "Antibiotic", "Recurrent vulvovaginal candidiasis, highly selective azole VORA", "600mg", 280.00f, "Rx" },
    { "Terbinafine (Lamisil oral)", "Antibiotic", "Onychomycosis and tinea, squalene epoxidase inhibitor allylamine", "250mg", 18.00f, "Rx" },
    { "Nystatin (Mycostatin oral)", "Antibiotic", "Oropharyngeal and GI candidiasis, polyene non-systemic antifungal", "500000 units", 12.00f, "Rx" },
    { "Griseofulvin (Grifulvin V)", "Antibiotic", "Tinea capitis and corporis, microtubule disrupting antifungal", "500mg", 18.00f, "Rx" },
    { "Natamycin (Natacyn)", "Antibiotic", "Fungal keratitis, topical polyene ophthalmic antifungal", "5%", 45.00f, "Rx" },
    { "Chloroquine (Aralen)", "Antibiotic", "Malaria treatment/prophylaxis and lupus, aminoquinoline", "500mg", 12.00f, "Rx" },
    { "Mefloquine (Lariam)", "Antibiotic", "Malaria prophylaxis resistant areas, quinolyl-methanol antimalarial", "250mg weekly", 28.00f, "Rx" },
    { "Atovaquone/Proguanil (Malarone)", "Antibiotic", "Malaria prophylaxis and treatment, bc1 complex inhibitor combo", "250/100mg", 28.00f, "Rx" },
    { "Artemether/Lumefantrine (Coartem)", "Antibiotic", "Uncomplicated P. falciparum malaria, artemisinin combination", "20/120mg", 38.00f, "Rx" },
    { "Artesunate (Artesunate-Avandia)", "Antibiotic", "Severe malaria IV, artemisinin derivative potent antiparasitic", "60mg IV", 180.00f, "Rx" },
    { "Tafenoquine (Krintafel)", "Antibiotic", "P. vivax radical cure prevention relapse, 8-aminoquinoline", "300mg", 85.00f, "Rx" },
    { "Primaquine (P. vivax)", "Antibiotic", "P. vivax/ovale radical cure and gametocytocidal, 8-aminoquinoline", "15mg", 8.00f, "Rx" },
    { "Miltefosine (Impavido)", "Antibiotic", "Visceral and cutaneous leishmaniasis, alkylphosphocholine oral", "50mg", 580.00f, "Rx" },
    { "Nitazoxanide (Alinia)", "Antibiotic", "Cryptosporidiosis and giardia, nitrothiazole PFOR inhibitor", "500mg", 28.00f, "Rx" },
    { "Tinidazole (Tindamax)", "Antibiotic", "Giardia, amoebiasis, BV, nitroimidazole broader spectrum", "500mg", 42.00f, "Rx" },
    { "Benznidazole (Rochagan)", "Antibiotic", "Chagas disease acute/chronic, nitroimidazole antiprotozoal", "100mg", 180.00f, "Rx" },
    { "Nifurtimox (Lampit)", "Antibiotic", "Chagas disease and sleeping sickness, nitrofuran antiprotozoal", "10mg/kg", 145.00f, "Rx" },
    { "Zinc Sulfate (Zincate)", "Vitamin", "Zinc deficiency, essential trace mineral cofactor supplement", "220mg", 6.99f, "OTC" },
    { "Ferrous Sulfate", "Vitamin", "Iron deficiency anemia, most common oral iron supplement", "325mg", 5.99f, "OTC" },
    { "Ferrous Gluconate (Fergon)", "Vitamin", "Iron deficiency anemia, gentler oral iron supplement form", "240mg", 6.99f, "OTC" },
    { "Ferrous Fumarate (Ferretts)", "Vitamin", "Iron deficiency anemia, high elemental iron oral supplement", "200mg", 6.99f, "OTC" },
    { "Ferric Maltol (Accrufer)", "Vitamin", "Iron deficiency including IBD, stable oral ferric iron complex", "30mg", 85.00f, "Rx" },
    { "Iron Polysaccharide (Niferex)", "Vitamin", "Iron deficiency, complex polysaccharide iron supplement", "150mg", 12.99f, "OTC" },
    { "Calcium Carbonate (Tums)", "Vitamin", "Antacid and calcium supplement, cheapest calcium form", "500mg", 7.99f, "OTC" },
    { "Calcium Citrate (Citracal)", "Vitamin", "Calcium supplement, better absorbed without food or acid", "250mg", 12.99f, "OTC" },
    { "Magnesium Oxide (Mag-Ox)", "Vitamin", "Magnesium deficiency and constipation, high Mg% content", "400mg", 6.99f, "OTC" },
    { "Magnesium Glycinate (Magtein)", "Vitamin", "Magnesium deficiency and sleep, chelated form less GI upset", "200mg", 14.99f, "OTC" },
    { "Potassium Gluconate", "Vitamin", "Mild hypokalemia prevention, OTC potassium supplementation", "99mg", 8.99f, "OTC" },
    { "Chromium Picolinate", "Vitamin", "Insulin sensitivity and glucose metabolism trace mineral", "200mcg", 7.99f, "OTC" },
    { "Selenium (Se)", "Vitamin", "Antioxidant and thyroid function, essential trace mineral", "200mcg", 6.99f, "OTC" },
    { "Iodine/Potassium Iodide (Iosat)", "Vitamin", "Iodine deficiency and thyroid radiation protection", "130mg", 8.99f, "OTC" },
    { "Copper Supplement (Cupric Oxide)", "Vitamin", "Copper deficiency and Menkes disease, essential trace mineral", "2mg", 5.99f, "OTC" },
    { "Manganese Supplement", "Vitamin", "Manganese deficiency and bone metabolism, trace mineral", "2.3mg", 5.99f, "OTC" },
    { "Molybdenum Supplement", "Vitamin", "Molybdenum deficiency, cofactor for sulfite oxidase enzyme", "45mcg", 5.99f, "OTC" },
    { "Biotin (Vitamin H)", "Vitamin", "Biotin deficiency and nail/hair health, B7 coenzyme", "5mg", 9.99f, "OTC" },
    { "Inositol (B8)", "Vitamin", "PCOS and anxiety, phospholipid precursor and cell signaling", "2g", 12.99f, "OTC" },
    { "Alpha-Lipoic Acid (ALA)", "Vitamin", "Diabetic neuropathy and antioxidant, universal antioxidant cofactor", "600mg", 14.99f, "OTC" },
    { "Coenzyme Q10 (CoQ10)", "Vitamin", "Statin myopathy mitochondrial support, ubiquinone electron carrier", "200mg", 19.99f, "OTC" },
    { "Fish Oil Omega-3 (Lovaza)", "Vitamin", "Hypertriglyceridemia and CV health, EPA and DHA supplement", "4g", 22.00f, "OTC" },
    { "Flaxseed Oil (ALA source)", "Vitamin", "Cardiovascular health and inflammation, plant-based omega-3 ALA", "1000mg", 9.99f, "OTC" },
    { "Melatonin Extended Release", "Vitamin", "Insomnia and jet lag, prolonged-release pineal hormone supplement", "2mg ER", 9.99f, "OTC" },
    { "Valerian Root (Valeriana)", "Vitamin", "Insomnia and anxiety, herbal GABA-A modulator sedative", "500mg", 8.99f, "OTC" },
    { "St. John's Wort (Hypericum)", "Vitamin", "Mild depression, herbal serotonin reuptake inhibitor hyperforin", "300mg", 9.99f, "OTC" },
    { "SAMe (S-Adenosylmethionine)", "Vitamin", "Depression and osteoarthritis, methyl donor cellular cofactor", "400mg", 24.99f, "OTC" },
    { "N-Acetylcysteine (NAC)", "Vitamin", "Acetaminophen overdose antidote and COPD mucus, cysteine precursor", "600mg", 12.99f, "OTC" },
    { "L-Carnitine (Carnitor)", "Vitamin", "Carnitine deficiency and renal dialysis, amino acid transport fatty acids", "330mg", 14.99f, "Rx" },
    { "Taurine Supplement", "Vitamin", "Energy and cardiac support, conditionally essential amino acid", "1g", 8.99f, "OTC" },
    { "Quercetin", "Vitamin", "Anti-inflammatory flavonoid, antioxidant mast cell stabilizer", "500mg", 14.99f, "OTC" },
    { "Resveratrol", "Vitamin", "Antioxidant and cardiovascular, stilbene polyphenol SIRT1 activator", "250mg", 14.99f, "OTC" },
    { "Curcumin (Turmeric Extract)", "Vitamin", "Anti-inflammatory and antioxidant, NF-kB and COX inhibitor", "500mg", 14.99f, "OTC" },
    { "Berberine", "Vitamin", "T2DM and cholesterol, plant alkaloid AMPK activator", "500mg", 19.99f, "OTC" },
    { "Ashwagandha (Withania somnifera)", "Vitamin", "Stress and cortisol reduction, adaptogenic herb withanolides", "600mg", 14.99f, "OTC" },
    { "Ginkgo Biloba Extract", "Vitamin", "Cognitive function and dementia prevention, flavone terpenoids", "120mg", 12.99f, "OTC" },
    { "Ginseng (Panax ginseng)", "Vitamin", "Energy, immune, and cognitive adaptogen ginsenosides", "200mg", 12.99f, "OTC" },
    { "Echinacea Extract", "Vitamin", "Immune stimulant common cold prevention, alkylamide immunomodulator", "400mg", 9.99f, "OTC" },
    { "Elderberry (Sambucol)", "Vitamin", "Antiviral immune support, anthocyanin flavonoid extract", "500mg", 14.99f, "OTC" },
    { "Probiotics (Lactobacillus/Bifidobacterium)", "Vitamin", "Gut microbiome and immune support, live bacterial culture blend", "10 billion CFU", 22.99f, "OTC" },
    { "Prebiotics (Inulin/FOS)", "Vitamin", "Gut microbiome health, fermentable fiber feeds beneficial bacteria", "5g", 14.99f, "OTC" },
    { "Collagen Peptides", "Vitamin", "Joint health and skin elasticity, hydrolyzed type I/II collagen", "10g", 22.99f, "OTC" },
    { "Glucosamine Sulfate (Dona)", "Vitamin", "Osteoarthritis symptom relief, amino sugar cartilage substrate", "1500mg", 22.99f, "OTC" },
    { "Chondroitin Sulfate", "Vitamin", "Osteoarthritis, glycosaminoglycan cartilage extracellular matrix", "1200mg", 19.99f, "OTC" },
    { "MSM (Methylsulfonylmethane)", "Vitamin", "Joint pain and inflammation, organic sulfur compound supplement", "1g", 12.99f, "OTC" },
    { "Hyaluronic Acid Oral (BioCell)", "Vitamin", "Joint lubrication and skin hydration, glycosaminoglycan oral", "200mg", 19.99f, "OTC" },
    { "Saw Palmetto (Serenoa repens)", "Vitamin", "BPH symptom relief, 5-alpha-reductase inhibitor herbal", "320mg", 14.99f, "OTC" },
    { "Pygeum Africanum Extract", "Vitamin", "BPH urinary symptoms, bark phytosterol anti-proliferative", "100mg", 12.99f, "OTC" },
    { "Pumpkin Seed Oil", "Vitamin", "OAB and BPH, phytosterol delta-7 sterine supplement", "1000mg", 14.99f, "OTC" },
    { "Cranberry Extract (PAC 36mg)", "Vitamin", "UTI prevention, proanthocyanidin anti-adhesion urinary", "500mg", 12.99f, "OTC" },
    { "D-Mannose", "Vitamin", "UTI prevention, simple sugar anti-adhesion E. coli urinary", "2g", 14.99f, "OTC" },
    { "Bromelain", "Vitamin", "Anti-inflammatory and sinus, pineapple stem cysteine protease", "500mg", 12.99f, "OTC" },
    { "Serrapeptase", "Vitamin", "Sinus and inflammation, serratiopeptidase proteolytic enzyme", "10mg", 14.99f, "OTC" },
    { "Nattokinase", "Vitamin", "Cardiovascular fibrinolytic, Bacillus subtilis natto serine protease", "100mg", 16.99f, "OTC" },
    { "Dimethyl Sulfoxide (DMSO)", "Skin", "Interstitial cystitis bladder instillation, solvent anti-inflammatory", "50% solution", 22.00f, "Rx" },
    { "Pentosan Polysulfate (Elmiron)", "Urology", "Interstitial cystitis, semi-synthetic heparin-like compound bladder", "100mg", 48.00f, "Rx" },
    { "Botulinum Toxin Bladder (Botox-OAB)", "Urology", "OAB and neurogenic detrusor overactivity, intradetrusor BoNT-A", "100units intravesical", 380.00f, "Rx" },
    { "Bacillus Calmette-Guerin (BCG bladder)", "Oncology", "Non-muscle-invasive bladder cancer, intravesical immunotherapy", "81mg intravesical", 380.00f, "Rx" },
    { "Mitomycin C (Jelmyto)", "Oncology", "Low-grade upper tract urothelial cancer, pyelocalyceal mitomycin gel", "4mg/mL", 1200.00f, "Rx" },
    { "Gemcitabine Intravesical", "Oncology", "NMIBC, intravesical nucleoside analogue chemotherapy", "2g intravesical", 380.00f, "Rx" },
    { "Valrubicin (Valstar)", "Oncology", "BCG-refractory bladder CIS, anthracycline intravesical instillation", "200mg intravesical", 680.00f, "Rx" },
    { "Cisplatin (Platinol)", "Oncology", "Testicular, bladder, lung, ovarian cancers, platinum alkylator IV", "75mg/m2 IV", 45.00f, "Rx" },
    { "Carboplatin (Paraplatin)", "Oncology", "Ovarian, lung, and head neck cancers, second-gen platinum IV", "AUC 5-6 IV", 85.00f, "Rx" },
    { "Oxaliplatin (Eloxatin)", "Oncology", "Colorectal cancer, third-gen platinum DACH derivative IV", "85mg/m2 IV", 145.00f, "Rx" },
    { "Doxorubicin (Adriamycin)", "Oncology", "Broad antitumor anthracycline, intercalating topoisomerase II IV", "60mg/m2 IV", 85.00f, "Rx" },
    { "Daunorubicin (Cerubidine)", "Oncology", "AML induction, anthracycline DNA intercalator topoisomerase II IV", "45mg/m2 IV", 95.00f, "Rx" },
    { "Epirubicin (Ellence)", "Oncology", "Breast cancer adjuvant, less cardiotoxic anthracycline epimer IV", "60mg/m2 IV", 95.00f, "Rx" },
    { "Idarubicin (Idamycin)", "Oncology", "AML, highly lipophilic oral-capable anthracycline IV", "12mg/m2 IV", 145.00f, "Rx" },
    { "Mitoxantrone (Novantrone)", "Oncology", "AML, prostate cancer, and MS relapse, anthracenedione IV", "12mg/m2 IV", 85.00f, "Rx" },
    { "Bleomycin (Blenoxane)", "Oncology", "Lymphoma and germ cell tumors, glycopeptide DNA strand break IV", "15units IM", 45.00f, "Rx" },
    { "Paclitaxel (Taxol)", "Oncology", "Breast, ovarian, NSCLC, microtubule stabilizer Cremophor IV", "175mg/m2 IV", 145.00f, "Rx" },
    { "Nab-Paclitaxel (Abraxane)", "Oncology", "Breast, NSCLC, pancreatic, albumin-bound paclitaxel nanoparticle", "260mg/m2 IV", 2800.00f, "Rx" },
    { "Docetaxel (Taxotere)", "Oncology", "Breast, prostate, NSCLC, GC, taxane microtubule stabilizer IV", "75mg/m2 IV", 185.00f, "Rx" },
    { "Cabazitaxel (Jevtana)", "Oncology", "Metastatic castration-resistant prostate cancer, taxane IV", "20mg/m2 IV", 8800.00f, "Rx" },
    { "Vinblastine (Velban)", "Oncology", "Hodgkin lymphoma, testicular, bladder, vinca alkaloid IV", "6mg/m2 IV", 45.00f, "Rx" },
    { "Vincristine (Oncovin)", "Oncology", "ALL, lymphomas, sarcoma, vinca alkaloid tubulin depolymerizer IV", "1.4mg/m2 IV", 45.00f, "Rx" },
    { "Vinorelbine (Navelbine)", "Oncology", "NSCLC and breast cancer, third-gen vinca alkaloid IV/oral", "25mg/m2 IV", 85.00f, "Rx" },
    { "Etoposide (VePesid)", "Oncology", "Lung, testicular, lymphoma, topoisomerase II inhibitor IV/oral", "50mg/m2 IV", 45.00f, "Rx" },
    { "Irinotecan (Camptosar)", "Oncology", "Colorectal cancer, topoisomerase I inhibitor prodrug IV", "125mg/m2 IV", 85.00f, "Rx" },
    { "Topotecan (Hycamtin)", "Oncology", "Ovarian and SCLC, topoisomerase I inhibitor IV/oral", "1.5mg/m2 IV", 95.00f, "Rx" },
    { "Temozolomide (Temodar)", "Oncology", "GBM and anaplastic astrocytoma, oral alkylating prodrug", "150mg/m2", 285.00f, "Rx" },
    { "Carmustine (BiCNU)", "Oncology", "Brain tumors and lymphoma, nitrosourea alkylating agent", "200mg/m2 IV", 145.00f, "Rx" },
    { "Lomustine (Gleostine)", "Oncology", "Brain tumors, nitrosourea alkylating oral agent", "130mg/m2", 1800.00f, "Rx" },
    { "Procarbazine (Matulane)", "Oncology", "Hodgkin lymphoma in MOPP, methylhydrazine alkylating MAOI", "50mg", 85.00f, "Rx" },
    { "Dacarbazine (DTIC)", "Oncology", "Melanoma and Hodgkin lymphoma, triazene alkylating agent IV", "850mg/m2 IV", 85.00f, "Rx" },
    { "Bendamustine (Treanda)", "Oncology", "CLL and NHL, bifunctional alkylator with purine analogue IV", "90mg/m2 IV", 680.00f, "Rx" },
    { "Melphalan (Alkeran)", "Oncology", "Myeloma and conditioning, phenyl alanine mustard alkylator", "8mg/m2", 185.00f, "Rx" },
    { "Busulfan (Myleran)", "Oncology", "CML and conditioning, bifunctional alkylating sulfonylalkane", "2mg", 85.00f, "Rx" },
    { "Chlorambucil (Leukeran)", "Oncology", "CLL and lymphomas, nitrogen mustard oral alkylating agent", "2mg", 45.00f, "Rx" },
    { "Cyclophosphamide (Cytoxan)", "Oncology", "Broad antitumor and autoimmune, nitrogen mustard prodrug", "500mg/m2 IV", 45.00f, "Rx" },
    { "Ifosfamide (Ifex)", "Oncology", "Sarcoma and testicular cancer, cyclophosphamide analog IV", "1.2g/m2 IV", 85.00f, "Rx" },
    { "Methotrexate (Rheumatrex)", "Oncology", "Leukemia, lymphoma, RA, psoriasis, dihydrofolate reductase inhibitor", "2.5mg weekly", 12.00f, "Rx" },
    { "Pemetrexed (Alimta)", "Oncology", "NSCLC and mesothelioma, multi-targeted antifolate IV", "500mg/m2 IV", 1800.00f, "Rx" },
    { "Cytarabine (Ara-C)", "Oncology", "AML and ALL, pyrimidine nucleoside antimetabolite IV", "100mg/m2 IV", 45.00f, "Rx" },
    { "Gemcitabine (Gemzar)", "Oncology", "NSCLC, pancreatic, bladder, breast, nucleoside analogue IV", "1000mg/m2 IV", 85.00f, "Rx" },
    { "Fludarabine (Fludara)", "Oncology", "CLL and NHL, adenine nucleoside analogue IV/oral", "25mg/m2 IV", 95.00f, "Rx" },
    { "Cladribine (Leustatin)", "Oncology", "Hairy cell leukemia, 2-CdA deoxyadenosine analogue IV", "0.09mg/kg/day IV", 1200.00f, "Rx" },
    { "Pentostatin (Nipent)", "Oncology", "Hairy cell leukemia and T-cell lymphoma, ADA inhibitor IV", "4mg/m2 IV", 850.00f, "Rx" },
    { "Nelarabine (Arranon)", "Oncology", "T-cell ALL/lymphoblastic lymphoma, deoxyguanosine analogue IV", "1500mg/m2 IV", 850.00f, "Rx" },
    { "Clofarabine (Clolar)", "Oncology", "Pediatric ALL relapsed, next-gen nucleoside analogue IV", "52mg/m2 IV", 1200.00f, "Rx" },
    { "Decitabine (Dacogen)", "Oncology", "MDS and AML, DNA hypomethylating nucleoside analogue IV", "15mg/m2 IV", 480.00f, "Rx" },
    { "Azacitidine (Vidaza)", "Oncology", "MDS and AML, DNA methyltransferase inhibitor SC/IV", "75mg/m2 SC", 480.00f, "Rx" },
    { "Oral Azacitidine (Onureg)", "Oncology", "AML maintenance, oral azacitidine hypomethylating agent", "300mg", 8500.00f, "Rx" },
    { "Lenalidomide (Revlimid)", "Oncology", "Myeloma, MDS del5q, lymphoma, cereblon E3 ligase modulator", "10mg", 680.00f, "Rx" },
    { "Pomalidomide (Pomalyst)", "Oncology", "Relapsed multiple myeloma, third-gen IMiD cereblon modulator", "4mg", 980.00f, "Rx" },
    { "Thalidomide (Thalomid)", "Oncology", "Myeloma and ENL, original cereblon modulator/antiangiogenic", "200mg", 380.00f, "Rx" },
    { "Arsenic Trioxide (Trisenox)", "Oncology", "APL relapsed, differentiating PML-RARa degradation inducer", "0.15mg/kg IV", 480.00f, "Rx" },
    { "All-Trans Retinoic Acid (Vesanoid)", "Oncology", "APL differentiation therapy, vitamin A receptor agonist oral", "45mg/m2", 145.00f, "Rx" },
    { "Ixazomib (Ninlaro)", "Oncology", "Multiple myeloma, oral proteasome inhibitor boronic acid", "4mg", 1800.00f, "Rx" },
    { "Carfilzomib (Kyprolis)", "Oncology", "Multiple myeloma, second-gen proteasome inhibitor irreversible IV", "27mg/m2 IV", 1400.00f, "Rx" },
    { "Belinostat (Beleodaq)", "Oncology", "Peripheral T-cell lymphoma, pan-HDAC inhibitor IV", "1000mg/m2 IV", 1200.00f, "Rx" },
    { "Romidepsin (Istodax)", "Oncology", "Cutaneous/peripheral T-cell lymphoma, cyclic peptide HDAC inhibitor", "14mg/m2 IV", 1800.00f, "Rx" },
    { "Vorinostat (Zolinza)", "Oncology", "CTCL, first approved HDAC inhibitor oral hydroxamate", "400mg", 1200.00f, "Rx" },
    { "Panobinostat (Farydak)", "Oncology", "Multiple myeloma, pan-HDAC inhibitor oral with bortezomib", "20mg", 1400.00f, "Rx" },
    { "Entinostat", "Oncology", "HR+ breast cancer, class I HDAC inhibitor in trials", "5mg weekly", 0.00f, "Rx" },
    { "Mocetinostat", "Oncology", "MDS and lymphoma, class I HDAC inhibitor in trials", "70mg", 0.00f, "Rx" },
    { "Tazemetostat (Tazverik)", "Oncology", "Follicular lymphoma and ES, EZH2 histone methyltransferase inhibitor", "800mg", 1800.00f, "Rx" },
    { "Olutasidenib (IDH1-AML)", "Oncology", "AML IDH1 mutated, differentiation agent oral inhibitor", "150mg", 2000.00f, "Rx" },
    { "Glasdegib (Daurismo)", "Oncology", "AML with hypomethylating agent, SMO/Hedgehog pathway inhibitor", "100mg", 1400.00f, "Rx" },
    { "Gilteritinib (Xospata)", "Oncology", "FLT3-mutated AML relapsed/refractory, FLT3/AXL inhibitor", "120mg", 1800.00f, "Rx" },
    { "Midostaurin (Rydapt)", "Oncology", "FLT3-mutated AML and mastocytosis, multi-kinase FLT3 inhibitor", "25mg", 1600.00f, "Rx" },
    { "Quizartinib (Vanflyta)", "Oncology", "FLT3-ITD AML, type II FLT3 inhibitor", "30mg", 1700.00f, "Rx" },
    { "Crenolanib", "Oncology", "FLT3-mutated AML including D835 resistance, type I FLT3i", "100mg", 0.00f, "Rx" },
    { "Fadraciclib", "Oncology", "CDK inhibitor cancer in trials, cell cycle kinase inhibitor", "100mg", 0.00f, "Rx" },
    { "Trilaciclib (Cosela)", "Oncology", "Myeloprotection during chemotherapy, CDK4/6 inhibitor IV", "240mg/m2 IV", 4800.00f, "Rx" },
    { "Olaparib/Bevacizumab (Lynparza-combo)", "Oncology", "BRCA-mutated ovarian cancer maintenance combination", "300/7.5mg", 2800.00f, "Rx" },
    { "Lutetium-177 PSMA (Pluvicto)", "Oncology", "PSMA-positive mCRPC, targeted radioligand therapy lutetium-177", "7.4GBq IV", 42000.00f, "Rx" },
    { "Lutetium-177 DOTATATE (Lutathera)", "Oncology", "Somatostatin receptor+ NETs, targeted radioligand therapy", "7.4GBq IV", 46000.00f, "Rx" },
    { "Radium-223 (Xofigo)", "Oncology", "Bone metastases mCRPC, alpha-emitting radiopharmaceutical", "55kBq/kg IV", 12000.00f, "Rx" },
    { "Ibritumomab Tiuxetan (Zevalin)", "Oncology", "NHL relapsed, radiolabeled anti-CD20 Yttrium-90 radioimmuno", "0.4mCi/kg IV", 3200.00f, "Rx" },
    { "Tositumomab/I-131 (Bexxar-d/c)", "Oncology", "Follicular NHL, I-131 radiolabeled anti-CD20 discontinued", "450mg IV", 0.00f, "Rx" },
    { "Apalutamide (Erleada)", "Men's Health", "Non-metastatic CRPC and mCSPC, next-gen AR antagonist", "240mg", 680.00f, "Rx" },
    { "Darolutamide (Nubeqa)", "Men's Health", "nmCRPC, novel structurally distinct AR antagonist minimal CNS", "300mg", 680.00f, "Rx" },
    { "Enzalutamide (Xtandi)", "Men's Health", "CRPC and mCSPC, potent androgen receptor signaling inhibitor", "160mg", 680.00f, "Rx" },
    { "Abiraterone (Zytiga)", "Men's Health", "CRPC, CYP17A1 steroidogenesis inhibitor with prednisone", "1000mg", 380.00f, "Rx" },
    { "Ketoconazole (low-dose CRPC)", "Men's Health", "CRPC off-label steroidogenesis suppression, CYP17 inhibitor", "400mg", 28.00f, "Rx" },
    { "Cabazitaxel (CRPC)", "Men's Health", "mCRPC post-docetaxel, taxane tubulin stabilizer resistance", "20mg/m2 IV", 8800.00f, "Rx" },
    { "Sipuleucel-T (Provenge)", "Men's Health", "Asymptomatic mCRPC, autologous PAP-GM-CSF DC vaccine", "single infusions", 93000.00f, "Rx" },
    { "Estramustine (Emcyt)", "Men's Health", "CRPC, estrogen/nitrogen mustard hybrid alkylating agent", "140mg", 85.00f, "Rx" },
    { "Letrozole (Femara)", "Women's Health", "HR+ breast cancer postmenopause, third-gen non-steroidal aromatase inhibitor", "2.5mg", 18.00f, "Rx" },
    { "Anastrozole (Arimidex)", "Women's Health", "HR+ breast cancer postmenopause, potent non-steroidal AI", "1mg", 18.00f, "Rx" },
    { "Exemestane (Aromasin)", "Women's Health", "HR+ breast cancer, steroidal irreversible aromatase inactivator", "25mg", 22.00f, "Rx" },
    { "Tamoxifen (Nolvadex)", "Women's Health", "Breast cancer prevention/treatment, SERM ER partial antagonist", "20mg", 12.00f, "Rx" },
    { "Toremifene (Fareston)", "Women's Health", "HR+ metastatic breast cancer in women, SERM tamoxifen-related", "60mg", 28.00f, "Rx" },
    { "Fulvestrant (Faslodex)", "Women's Health", "HR+ HER2- breast cancer, ER degrader SERD pure antagonist", "500mg IM", 680.00f, "Rx" },
    { "Elacestrant (Orserdu)", "Women's Health", "ESR1-mutated HR+ breast cancer, oral SERD ER degrader", "345mg", 1800.00f, "Rx" },
    { "Camizestrant", "Women's Health", "HR+ breast cancer next-gen oral SERD in trials", "75mg", 0.00f, "Rx" },
    { "Inavolisib (Itovebi)", "Women's Health", "PIK3CA-mutated HR+ breast cancer with CDK4/6 inhibitor", "9mg", 1600.00f, "Rx" },
    { "Clomiphene (Clomid)", "Women's Health", "Anovulatory infertility, SERM ovarian stimulation", "50mg", 18.00f, "Rx" },
    { "Letrozole (Femara-infertility)", "Women's Health", "Ovulation induction in PCOS preferred over clomiphene", "2.5mg", 18.00f, "Rx" },
    { "Gonadotropin FSH (Gonal-F)", "Women's Health", "IVF ovarian stimulation, recombinant follitropin alpha SC", "75units SC", 180.00f, "Rx" },
    { "Gonadotropin LH (Luveris)", "Women's Health", "IVF with FSH co-stimulation, recombinant lutropin alfa SC", "75units SC", 145.00f, "Rx" },
    { "hCG (Pregnyl)", "Women's Health", "IVF ovulation trigger, human chorionic gonadotropin injection", "5000units IM", 45.00f, "Rx" },
    { "Progesterone Vaginal (Crinone)", "Women's Health", "Luteal support IVF and threatened abortion, vaginal progesterone", "8%", 38.00f, "Rx" },
    { "Leuprolide (Lupron Depot)", "Women's Health", "Endometriosis, fibroids, prostate cancer, GnRH agonist monthly depot", "3.75mg IM", 480.00f, "Rx" },
    { "Elagolix (Orilissa)", "Women's Health", "Endometriosis pain, oral non-peptide GnRH receptor antagonist", "150mg", 680.00f, "Rx" },
    { "Relugolix (Orgovyx)", "Women's Health", "Prostate cancer and uterine fibroids, oral GnRH receptor antagonist", "120mg", 680.00f, "Rx" },
    { "Linzagolix (Yselty)", "Women's Health", "Uterine fibroids with add-back, oral GnRH antagonist", "200mg", 650.00f, "Rx" },
    { "Ulipristal Acetate (ella)", "Women's Health", "Emergency contraception and fibroids, selective PR modulator", "30mg", 28.00f, "Rx" },
    { "Levonorgestrel (Plan B)", "Women's Health", "Emergency contraception within 72h, progestin prevents ovulation", "1.5mg", 14.99f, "OTC" },
    { "Mifepristone (Mifeprex)", "Women's Health", "Medical abortion with misoprostol, anti-progestin antagonist", "200mg", 95.00f, "Rx" },
    { "Misoprostol (Cytotec)", "Women's Health", "Cervical ripening, PPH, medical abortion, PGE1 prostaglandin", "200mcg", 12.00f, "Rx" },
    { "Carboprost (Hemabate)", "Women's Health", "Postpartum hemorrhage, 15-methyl PGF2-alpha uterotonic", "250mcg IM", 28.00f, "Rx" },
    { "Oxytocin (Pitocin)", "Women's Health", "Labor induction and PPH prevention, synthetic oxytocin IV", "10units/mL IV", 12.00f, "Rx" },
    { "Dinoprostone (Cervidil)", "Women's Health", "Cervical ripening, PGE2 prostaglandin vaginal insert", "10mg vaginal", 28.00f, "Rx" },
    { "Conjugated Estrogens (Premarin)", "Women's Health", "Menopause symptoms and osteoporosis, equine estrogen mixture", "0.625mg", 18.00f, "Rx" },
    { "Estradiol Patch (Vivelle-Dot)", "Women's Health", "Menopause, transdermal 17-beta estradiol twice-weekly patch", "0.05mg/day", 28.00f, "Rx" },
    { "Progesterone Micronized (Prometrium)", "Women's Health", "Menopause HRT protection and luteal phase, oral progesterone", "200mg", 22.00f, "Rx" },
    { "Ospemifene (Osphena)", "Women's Health", "GSM dyspareunia, oral SERM with estrogenic vaginal effects", "60mg", 95.00f, "Rx" },
    { "Bazedoxifene/Estrogen (Duavee)", "Women's Health", "Menopause symptoms, TSEC combination estrogen plus SERM", "0.45/20mg", 95.00f, "Rx" },
    { "Norethindrone (Aygestin)", "Women's Health", "Endometriosis, abnormal uterine bleeding, oral progestin", "5mg", 22.00f, "Rx" },
    { "Etonogestrel Implant (Nexplanon)", "Women's Health", "Long-acting contraception 3 years, subdermal etonogestrel rod", "68mg implant", 280.00f, "Rx" },
    { "Levonorgestrel IUD (Mirena)", "Women's Health", "5-year contraception and heavy menstrual bleeding, LNG-IUS", "52mg IUD", 580.00f, "Rx" },
    { "Copper IUD (ParaGard)", "Women's Health", "10-year contraception and EC, non-hormonal copper device", "device", 380.00f, "Rx" },
    { "Norelgestromin/Ethinyl Estradiol Patch (Xulane)", "Women's Health", "Weekly contraceptive patch, progestin plus estrogen transdermal", "150/35mcg/day", 28.00f, "Rx" },
    { "Etonogestrel/Ethinyl Estradiol Ring (NuvaRing)", "Women's Health", "Monthly vaginal contraceptive ring, ENG plus EE combination", "0.12/0.015mg/day", 38.00f, "Rx" },
    { "Segesterone/Ethinyl Estradiol Ring (Annovera)", "Women's Health", "Annual contraceptive vaginal ring, segesterone acetate plus EE", "0.15/0.013mg/day", 580.00f, "Rx" },};
static constexpr int MEDICINE_COUNT =
    static_cast<int>(sizeof(MEDICINES)/sizeof(MEDICINES[0]));

// ─── Utilities ────────────────────────────────────────────────────────────────
static std::string toLower(const std::string& s){
    std::string r=s;
    for(char& c:r) c=static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return r;
}
static std::string trunc(const std::string& s,size_t n){
    if(s.size()<=n) return s;
    return s.substr(0,n-2)+"..";
}

// ─── Search ───────────────────────────────────────────────────────────────────
static std::vector<int> doSearch(const std::string& q,
                                  const std::string& tf,
                                  const std::string& cf){
    std::string ql=toLower(q);
    std::vector<int> out;
    for(int i=0;i<MEDICINE_COUNT;++i){
        const Medicine& m=MEDICINES[i];
        if(!tf.empty()&&toLower(m.type)!=toLower(tf)) continue;
        if(!cf.empty()&&toLower(m.category)!=toLower(cf)) continue;
        if(ql.empty()||
           toLower(m.name).find(ql)!=std::string::npos||
           toLower(m.category).find(ql)!=std::string::npos||
           toLower(m.use).find(ql)!=std::string::npos)
            out.push_back(i);
    }
    return out;
}

// ─── Sort ────────────────────────────────────────────────────────────────────
enum class SortField{Name,Price,Category,Dosage};

static void doSort(std::vector<int>& idx, SortField f, bool asc){
    switch(f){
        case SortField::Name:
            std::sort(idx.begin(),idx.end(),[](int a,int b){
                return toLower(MEDICINES[a].name)<toLower(MEDICINES[b].name);});
            break;
        case SortField::Price:
            std::sort(idx.begin(),idx.end(),[](int a,int b){
                return MEDICINES[a].price<MEDICINES[b].price;});
            break;
        case SortField::Category:
            std::sort(idx.begin(),idx.end(),[](int a,int b){
                return toLower(MEDICINES[a].category)<toLower(MEDICINES[b].category);});
            break;
        case SortField::Dosage:
            std::sort(idx.begin(),idx.end(),[](int a,int b){
                return std::string(MEDICINES[a].dosage)<std::string(MEDICINES[b].dosage);});
            break;
    }
    if(!asc) std::reverse(idx.begin(),idx.end());
}

// ─── Draw helpers ─────────────────────────────────────────────────────────────
static void drawText(SDL_Renderer* ren,TTF_Font* font,
                     const std::string& text,int x,int y,SDL_Color col){
    if(!font||text.empty()) return;
    SDL_Surface* s=TTF_RenderText_Blended(font,text.c_str(),col);
    if(!s) return;
    SDL_Texture* t=SDL_CreateTextureFromSurface(ren,s);
    SDL_FreeSurface(s);
    if(!t) return;
    int w,h; SDL_QueryTexture(t,nullptr,nullptr,&w,&h);
    SDL_Rect dst{x,y,w,h};
    SDL_RenderCopy(ren,t,nullptr,&dst);
    SDL_DestroyTexture(t);
}
static int textW(TTF_Font* f,const std::string& s){
    if(!f||s.empty()) return 0;
    int w=0,h=0; TTF_SizeText(f,s.c_str(),&w,&h); return w;
}
static void fillCircle(SDL_Renderer* ren,int cx,int cy,int r){
    for(int dy=-r;dy<=r;++dy){
        int dx=static_cast<int>(std::sqrt(static_cast<double>(r*r-dy*dy)));
        SDL_RenderDrawLine(ren,cx-dx,cy+dy,cx+dx,cy+dy);
    }
}
static void drawEllipse(SDL_Renderer* ren,float cx,float cy,
                        float rx,float ry,float tilt,SDL_Color col){
    SDL_SetRenderDrawColor(ren,col.r,col.g,col.b,col.a);
    const int S=200;
    float px=0,py=0;
    for(int i=0;i<=S;++i){
        float t=i*2.f*static_cast<float>(M_PI)/S;
        float x=cx+rx*std::cos(t)*std::cos(tilt)-ry*std::sin(t)*std::sin(tilt);
        float y=cy+rx*std::cos(t)*std::sin(tilt)+ry*std::sin(t)*std::cos(tilt);
        if(i>0) SDL_RenderDrawLine(ren,(int)px,(int)py,(int)x,(int)y);
        px=x; py=y;
    }
}
static void drawThickEllipse(SDL_Renderer* ren,float cx,float cy,
                             float rx,float ry,float tilt,SDL_Color col,int thick){
    for(int d=-thick/2;d<=thick/2;++d){
        float sc=1.f+d*0.018f;
        float a=1.f-std::abs((float)d)/((float)(thick/2+1));
        SDL_Color c=col; c.a=(Uint8)(col.a*a);
        drawEllipse(ren,cx,cy,rx*sc,ry*sc,tilt,c);
    }
}
static TTF_Font* tryFont(int pt){
    const char* paths[]={
        "C:/Users/Karson/AppData/Local/Microsoft/Windows/Fonts/JetBrainsMonoNerdFont-Regular.ttf",
        "C:/Users/Karson/AppData/Local/Microsoft/Windows/Fonts/JetBrainsMono-Regular.ttf",
        "C:/Windows/Fonts/consola.ttf","C:/Windows/Fonts/cour.ttf",
        "C:/Windows/Fonts/lucon.ttf",
        "/usr/share/fonts/truetype/jetbrains-mono/JetBrainsMono-Regular.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
        "/System/Library/Fonts/Menlo.ttc","font.ttf",nullptr};
    for(int i=0;paths[i];++i){TTF_Font* f=TTF_OpenFont(paths[i],pt);if(f)return f;}
    return nullptr;
}
static TTF_Font* tryFontBold(int pt){
    const char* paths[]={
        "C:/Users/Karson/AppData/Local/Microsoft/Windows/Fonts/JetBrainsMonoNerdFont-Bold.ttf",
        "C:/Users/Karson/AppData/Local/Microsoft/Windows/Fonts/JetBrainsMonoNerdFont-SemiBold.ttf",
        "C:/Users/Karson/AppData/Local/Microsoft/Windows/Fonts/JetBrainsMono-Bold.ttf",
        "C:/Windows/Fonts/consolab.ttf","C:/Windows/Fonts/courbd.ttf",nullptr};
    for(int i=0;paths[i];++i){TTF_Font* f=TTF_OpenFont(paths[i],pt);if(f)return f;}
    return tryFont(pt);
}
static SDL_Color catColor(const char* cat){
    std::string c=toLower(cat);
    if(c=="pain relief")    return {220,215,210,255};
    if(c=="antibiotic")     return {210,225,210,255};
    if(c=="allergy")        return {215,215,228,255};
    if(c=="heart")          return {228,210,210,255};
    if(c=="diabetes")       return {210,228,215,255};
    if(c=="mental health")  return {220,215,228,255};
    if(c=="respiratory")    return {210,222,228,255};
    if(c=="neurological")   return {228,222,210,255};
    if(c=="oncology")       return {228,212,218,255};
    if(c=="vitamin")        return {228,228,210,255};
    if(c=="skin")           return {225,218,210,255};
    if(c=="antiviral")      return {212,228,220,255};
    if(c=="antifungal")     return {228,220,212,255};
    if(c=="blood")          return {230,210,212,255};
    if(c=="immune")         return {215,228,225,255};
    if(c=="hormonal")       return {228,215,228,255};
    if(c=="sleep")          return {210,215,228,255};
    if(c=="women's health") return {228,210,222,255};
    if(c=="men's health")   return {215,225,228,255};
    if(c=="bone")           return {228,225,215,255};
    if(c=="thyroid")        return {215,228,228,255};
    if(c=="emergency")      return {240,215,215,255};
    if(c=="muscle relaxant")return {220,215,220,255};
    if(c=="eye/ent")        return {215,220,228,255};
    return {200,200,200,255};
}

// ─── Category info for detail window ─────────────────────────────────────────
static const char* catInfo(const char* cat){
    std::string c=toLower(cat);
    if(c=="antibiotic")    return "Antibiotics treat bacterial infections. Always complete the full course. Do not use for viral infections. Overuse contributes to antibiotic resistance.";
    if(c=="pain relief")   return "Pain relievers reduce pain and inflammation. Follow dosage limits carefully. Long-term NSAID use can affect kidney and cardiovascular health.";
    if(c=="allergy")       return "Antihistamines and allergy drugs reduce immune responses to allergens. Some cause drowsiness. Discuss long-term use with a healthcare provider.";
    if(c=="heart")         return "Cardiovascular drugs treat hypertension, heart failure, arrhythmias, and cholesterol. Never stop abruptly without medical guidance.";
    if(c=="diabetes")      return "Diabetes medications control blood sugar levels. Monitor glucose regularly. Combine with diet and exercise for best outcomes.";
    if(c=="mental health") return "Psychiatric medications require careful dosing and monitoring. Never stop antidepressants abruptly. Effects may take weeks to appear.";
    if(c=="respiratory")   return "Respiratory drugs manage asthma, COPD, and allergic rhinitis. Inhaler technique significantly affects effectiveness.";
    if(c=="neurological")  return "Neurological drugs treat epilepsy, Parkinson's, migraines, and dementia. Many require therapeutic drug monitoring.";
    if(c=="oncology")      return "Cancer drugs target tumor cells but often affect healthy tissue. Side effects can be significant. Used under strict oncology supervision.";
    if(c=="vitamin")       return "Vitamins and supplements support health but don't replace a balanced diet. Fat-soluble vitamins (A,D,E,K) can accumulate — don't overdose.";
    if(c=="antiviral")     return "Antiviral drugs inhibit viral replication. Most effective when taken early. Resistance can develop with incomplete courses.";
    if(c=="antifungal")    return "Antifungal agents treat fungal infections of skin, nails, and internal organs. Liver function monitoring may be needed with systemic use.";
    if(c=="immune")        return "Immunosuppressants and biologics modify the immune system. Increased infection risk requires monitoring. Often used for autoimmune conditions.";
    if(c=="bone")          return "Bone medications prevent and treat osteoporosis and Paget disease. Take bisphosphonates with water and remain upright for 30 minutes.";
    if(c=="emergency")     return "Emergency drugs are used in life-threatening situations. Administered by trained healthcare providers. Rapid action is essential.";
    if(c=="sleep")         return "Sleep aids should be used short-term. Dependency can develop. Practice good sleep hygiene alongside medication use.";
    if(c=="skin")          return "Topical and systemic skin medications treat acne, eczema, psoriasis, and infections. Use as directed — overuse can cause skin thinning.";
    if(c=="blood")         return "Blood medications include anticoagulants, iron supplements, and clotting factors. Anticoagulants require regular INR monitoring.";
    if(c=="thyroid")       return "Thyroid medications restore hormone balance. Take on empty stomach, 30-60 min before eating. TSH monitoring every 6-12 months.";
    if(c=="digestive")     return "GI medications treat acid reflux, IBD, constipation, and nausea. Long-term PPI use may affect magnesium and B12 absorption.";
    if(c=="hormonal")      return "Hormonal medications affect the endocrine system. Used for contraception, HRT, and endocrine disorders. Regular monitoring essential.";
    return "Consult your healthcare provider for complete prescribing information, interactions, and contraindications for this medication.";
}

// ─── Detail window ────────────────────────────────────────────────────────────
static void renderDetailWindow(SDL_Renderer* ren, int medIdx,
                                TTF_Font* fBig, TTF_Font* fMed, TTF_Font* fSub){
    if(!ren||medIdx<0||medIdx>=MEDICINE_COUNT) return;
    const Medicine& m=MEDICINES[medIdx];

    // Background
    SDL_SetRenderDrawColor(ren,13,13,13,255);
    SDL_RenderClear(ren);

    // Top header bar
    SDL_SetRenderDrawColor(ren,20,20,20,255);
    SDL_Rect hdrBg{0,0,DET_W,80};
    SDL_RenderFillRect(ren,&hdrBg);
    SDL_SetRenderDrawColor(ren,42,42,42,255);
    SDL_RenderDrawLine(ren,0,80,DET_W,80);

    // Medicine name
    if(fBig) drawText(ren,fBig,m.name,20,14,{245,245,245,255});

    // Type badge
    SDL_Color tc=(std::string(m.type)=="OTC")
        ?SDL_Color{130,210,130,255}:SDL_Color{210,130,130,255};
    if(fSub){
        std::string badge=std::string("  ")+m.type+"  ";
        // Badge background
        int bw=textW(fSub,badge);
        SDL_SetRenderDrawColor(ren,tc.r/5,tc.g/5,tc.b/5,255);
        SDL_Rect bb{18,56,bw,16};
        SDL_RenderFillRect(ren,&bb);
        drawText(ren,fSub,badge,18,57,tc);
    }

    // Fields section
    int y=98;
    int labelX=20, valueX=200;
    SDL_Color labelCol={80,80,80,255};
    SDL_Color valueCol={210,210,210,255};
    auto field=[&](const char* label,const std::string& val,SDL_Color vc){
        if(fSub) drawText(ren,fSub,label,labelX,y,labelCol);
        if(fMed) drawText(ren,fMed,val,valueX,y-1,vc);
        SDL_SetRenderDrawColor(ren,28,28,28,255);
        SDL_RenderDrawLine(ren,20,y+20,DET_W-20,y+20);
        y+=30;
    };

    field("CATEGORY",  m.category, catColor(m.category));
    field("INDICATION",m.use,valueCol);
    field("DOSAGE",    m.dosage,valueCol);

    char pb[32];
    std::snprintf(pb,sizeof(pb),"$%.2f USD",m.price);
    field("PRICE",     pb,valueCol);
    field("CLASS",     (std::string(m.type)=="OTC")
                       ?"Over The Counter — available without prescription"
                       :"Prescription Required — consult a licensed provider",valueCol);

    // Category info box
    y+=12;
    SDL_SetRenderDrawColor(ren,22,22,22,255);
    SDL_Rect infoBox{20,y,DET_W-40,140};
    SDL_RenderFillRect(ren,&infoBox);
    SDL_SetRenderDrawColor(ren,35,35,35,255);
    SDL_RenderDrawRect(ren,&infoBox);

    if(fSub){
        drawText(ren,fSub,"CLINICAL NOTES",26,y+8,{60,60,60,255});
        // Word-wrap the info text (simple: split into ~75-char lines)
        std::string info=catInfo(m.category);
        int lineY=y+24;
        std::string word,line;
        int maxChars=72;
        for(size_t i=0;i<=info.size();++i){
            char ch=(i<info.size()?info[i]:' ');
            if(ch==' '||i==info.size()){
                if(line.size()+word.size()>(size_t)maxChars && !line.empty()){
                    drawText(ren,fSub,line,26,lineY,{90,90,90,255});
                    lineY+=15; line="";
                }
                if(!line.empty()) line+=" ";
                line+=word; word="";
            } else { word+=ch; }
        }
        if(!line.empty()) drawText(ren,fSub,line,26,lineY,{90,90,90,255});
    }

    // Footer
    SDL_SetRenderDrawColor(ren,18,18,18,255);
    SDL_Rect foot{0,DET_H-36,DET_W,36};
    SDL_RenderFillRect(ren,&foot);
    SDL_SetRenderDrawColor(ren,35,35,35,255);
    SDL_RenderDrawLine(ren,0,DET_H-37,DET_W,DET_H-37);
    if(fSub){
        drawText(ren,fSub,"This information is for reference only. Always consult a healthcare professional.",
                 20,DET_H-24,{50,50,50,255});
    }

    SDL_RenderPresent(ren);
}

// ─── Main ─────────────────────────────────────────────────────────────────────
int main(int,char**){
    if(SDL_Init(SDL_INIT_VIDEO)!=0) return 1;
    if(!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG)){SDL_Quit();return 1;}
    if(TTF_Init()!=0){IMG_Quit();SDL_Quit();return 1;}

    SDL_Window* win=SDL_CreateWindow("Orbit-Med",
        SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
        WIN_W,WIN_H,SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* ren=SDL_CreateRenderer(win,-1,
        SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_BLEND);

    TTF_Font* fSub=tryFont(12);
    TTF_Font* fMed=tryFont(14);
    TTF_Font* fBig=tryFontBold(30);

    // Detail window state
    SDL_Window*   detWin=nullptr;
    SDL_Renderer* detRen=nullptr;
    int           detMed=-1;

    auto openDetail=[&](int idx){
        if(detWin){SDL_DestroyRenderer(detRen);SDL_DestroyWindow(detWin);}
        detMed=idx;
        detWin=SDL_CreateWindow(MEDICINES[idx].name,
            SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
            DET_W,DET_H,SDL_WINDOW_SHOWN);
        detRen=SDL_CreateRenderer(detWin,-1,
            SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    };
    auto closeDetail=[&](){
        if(detRen){SDL_DestroyRenderer(detRen);detRen=nullptr;}
        if(detWin){SDL_DestroyWindow(detWin);detWin=nullptr;}
        detMed=-1;
    };

    // Main state
    std::string searchQuery;
    SortField   sortField=SortField::Name;
    bool        sortAsc=true;
    std::string typeFilter,catFilter;
    int         scrollOff=0;

    // Atom animation
    float ringTilt[3]={0.f,
                       static_cast<float>(M_PI)/3.f,
                       2.f*static_cast<float>(M_PI)/3.f};
    float eAngle[3]  ={0.f,
                       2.f*static_cast<float>(M_PI)/3.f,
                       4.f*static_cast<float>(M_PI)/3.f};
    const float rSpd[3]={1.0f,0.82f,1.18f};
    const float eSpd[3]={1.0f,0.90f,1.15f};
    float shimmerX=-SHIMMER_W,blinkT=0;

    SDL_StartTextInput();
    auto lastT=std::chrono::steady_clock::now();

    const int X_NAME=TABLE_X+COL_IDX;
    const int X_CAT =X_NAME+COL_NAME;
    const int X_USE =X_CAT +COL_CAT;
    const int X_DOS =X_USE +COL_USE;
    const int X_PRI =X_DOS +COL_DOS;
    const int X_TYPE=X_PRI +COL_PRI;

    struct C{static SDL_Color m(Uint8 r,Uint8 g,Uint8 b){return{r,g,b,255};}};
    auto BG    =C::m(11,11,11);
    auto PANEL =C::m(18,18,18);
    auto ROWALT=C::m(22,22,22);
    auto SEP   =C::m(42,42,42);
    auto WHITE =C::m(245,245,245);
    auto LGRAY =C::m(170,170,170);
    auto MGRAY =C::m(100,100,100);
    auto DGRAY =C::m(55,55,55);
    auto ACCENT=C::m(175,200,235);
    auto SBRD  =C::m(55,75,110);

    // Hover state
    int hoverRow=-1;

    bool running=true;
    while(running){
        auto now=std::chrono::steady_clock::now();
        float dt=std::chrono::duration<float>(now-lastT).count();
        lastT=now;
        if(dt>0.1f)dt=0.1f;

        // Mouse position for hover
        int mx=0,my=0;
        SDL_GetMouseState(&mx,&my);

        SDL_Event ev;
        while(SDL_PollEvent(&ev)){
            Uint32 mainID=SDL_GetWindowID(win);
            Uint32 detID=detWin?SDL_GetWindowID(detWin):0;

            if(ev.type==SDL_QUIT){
                if(detWin&&ev.window.windowID==detID) closeDetail();
                else running=false;
            }
            // Window close button
            if(ev.type==SDL_WINDOWEVENT&&
               ev.window.event==SDL_WINDOWEVENT_CLOSE){
                if(ev.window.windowID==detID) closeDetail();
                else running=false;
            }
            // Keyboard
            if(ev.type==SDL_KEYDOWN){
                Uint32 wid=ev.key.windowID;
                if(wid==detID){
                    if(ev.key.keysym.sym==SDLK_ESCAPE) closeDetail();
                } else {
                    switch(ev.key.keysym.sym){
                        case SDLK_ESCAPE: running=false; break;
                        case SDLK_BACKSPACE:
                            if(!searchQuery.empty()){searchQuery.pop_back();scrollOff=0;}
                            break;
                        case SDLK_DELETE: searchQuery.clear();scrollOff=0; break;
                        case SDLK_UP:    if(scrollOff>0)scrollOff--; break;
                        case SDLK_DOWN:  scrollOff++; break;
                        case SDLK_PAGEUP:   scrollOff-=15;if(scrollOff<0)scrollOff=0; break;
                        case SDLK_PAGEDOWN: scrollOff+=15; break;
                        case SDLK_HOME:  scrollOff=0; break;
                        // Sort keys — press same key again to toggle asc/desc
                        case SDLK_F1:
                            if(sortField==SortField::Name) sortAsc=!sortAsc;
                            else{sortField=SortField::Name;sortAsc=true;}
                            scrollOff=0; break;
                        case SDLK_F2:
                            if(sortField==SortField::Price) sortAsc=!sortAsc;
                            else{sortField=SortField::Price;sortAsc=true;}
                            scrollOff=0; break;
                        case SDLK_F3:
                            if(sortField==SortField::Category) sortAsc=!sortAsc;
                            else{sortField=SortField::Category;sortAsc=true;}
                            scrollOff=0; break;
                        case SDLK_F4:
                            if(sortField==SortField::Dosage) sortAsc=!sortAsc;
                            else{sortField=SortField::Dosage;sortAsc=true;}
                            scrollOff=0; break;
                        case SDLK_F5: typeFilter="";scrollOff=0; break;
                        case SDLK_F6: typeFilter="OTC";scrollOff=0; break;
                        case SDLK_F7: typeFilter="Rx";scrollOff=0; break;
                        default: break;
                    }
                }
            }
            // Text input (main window only)
            if(ev.type==SDL_TEXTINPUT){
                (void)0;
                if(!detWin||(SDL_GetMouseFocus()==win))
                    {searchQuery+=ev.text.text;scrollOff=0;}
            }
            // Mouse wheel
            if(ev.type==SDL_MOUSEWHEEL&&ev.wheel.windowID==mainID){
                scrollOff-=ev.wheel.y*3;
                if(scrollOff<0)scrollOff=0;
            }
            // Mouse click — open detail
            if(ev.type==SDL_MOUSEBUTTONDOWN&&
               ev.button.button==SDL_BUTTON_LEFT&&
               ev.button.windowID==mainID){
                int clkY=ev.button.y;
                int tBottom=WIN_H-28;
                if(clkY>TABLE_Y+ROW_H&&clkY<tBottom){
                    // Will be resolved after results are computed below
                    // Store the click Y for processing after sort
                    // Use a sentinel approach
                    hoverRow=clkY;  // repurpose as click signal
                }
            }
        }

        // Animate
        for(int i=0;i<3;++i){
            ringTilt[i]+=RING_SPIN*rSpd[i]*dt;
            eAngle[i]  +=ELEC_SPEED*eSpd[i]*dt;
        }
        shimmerX+=SHIMMER_SPEED*dt;
        if(shimmerX>WIN_W+SHIMMER_W) shimmerX=-SHIMMER_W;
        blinkT+=dt;

        // Results
        auto results=doSearch(searchQuery,typeFilter,catFilter);
        doSort(results,sortField,sortAsc);

        int maxVis=(WIN_H-TABLE_Y-ROW_H-30)/ROW_H;
        int maxSc=std::max(0,(int)results.size()-maxVis);
        if(scrollOff>maxSc)scrollOff=maxSc;

        // Process stored click
        if(hoverRow>0){
            int clkY=hoverRow; hoverRow=-1;
            int tBottom=WIN_H-28;
            if(clkY>TABLE_Y+ROW_H&&clkY<tBottom){
                int row=(clkY-TABLE_Y-ROW_H)/ROW_H+scrollOff;
                if(row>=0&&row<(int)results.size())
                    openDetail(results[row]);
            }
        }

        // Compute hover row from mouse
        {
            int tBottom=WIN_H-28;
            if(my>TABLE_Y+ROW_H&&my<tBottom&&mx>TABLE_X&&mx<WIN_W-TABLE_X)
                hoverRow=(my-TABLE_Y-ROW_H)/ROW_H+scrollOff;
            else hoverRow=-1;
        }

        // ─── Render main window ──────────────────────────────────────────────
        SDL_SetRenderDrawColor(ren,BG.r,BG.g,BG.b,255);
        SDL_RenderClear(ren);

        // Shimmer
        SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_ADD);
        for(int bx=0;bx<SHIMMER_W;++bx){
            float t=(float)bx/SHIMMER_W;
            float a=std::sin(t*static_cast<float>(M_PI));
            Uint8 av=(Uint8)(a*SHIMMER_BRIGHT);
            SDL_SetRenderDrawColor(ren,av,av,av,255);
            SDL_RenderDrawLine(ren,(int)shimmerX+bx,0,(int)shimmerX+bx,WIN_H);
        }
        SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_BLEND);

        // Header gradient
        for(int y=0;y<HEADER_H;++y){
            float t=(float)y/HEADER_H;
            Uint8 v=(Uint8)(24-t*10);
            SDL_SetRenderDrawColor(ren,v,v,v,255);
            SDL_RenderDrawLine(ren,0,y,WIN_W,y);
        }
        for(int i=0;i<4;++i){
            Uint8 v=(Uint8)(50-i*12);
            SDL_SetRenderDrawColor(ren,v,v,v,255);
            SDL_RenderDrawLine(ren,0,HEADER_H+i,WIN_W,HEADER_H+i);
        }

        // ── Atom ──────────────────────────────────────────────────────────────
        SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_ADD);
        for(int i=0;i<3;++i){
            SDL_Color glow={30,55,110,90};
            drawThickEllipse(ren,LOGO_CX,LOGO_CY,RING_RX+5,RING_RY+3,ringTilt[i],glow,8);
            SDL_Color ring={140,180,240,200};
            drawThickEllipse(ren,LOGO_CX,LOGO_CY,RING_RX,RING_RY,ringTilt[i],ring,3);
        }
        for(int i=0;i<3;++i){
            float ex=LOGO_CX+RING_RX*std::cos(eAngle[i])*std::cos(ringTilt[i])
                            -RING_RY*std::sin(eAngle[i])*std::sin(ringTilt[i]);
            float ey=LOGO_CY+RING_RX*std::cos(eAngle[i])*std::sin(ringTilt[i])
                            +RING_RY*std::sin(eAngle[i])*std::cos(ringTilt[i]);
            int ox=(int)ex,oy=(int)ey;
            SDL_SetRenderDrawColor(ren,40,80,180,80);
            fillCircle(ren,ox,oy,ELEC_R+9);
            SDL_SetRenderDrawColor(ren,100,150,230,160);
            fillCircle(ren,ox,oy,ELEC_R+4);
            SDL_SetRenderDrawColor(ren,210,230,255,255);
            fillCircle(ren,ox,oy,ELEC_R);
        }
        SDL_SetRenderDrawColor(ren,30,55,120,90);
        fillCircle(ren,LOGO_CX,LOGO_CY,NUCLEUS_R+10);
        SDL_SetRenderDrawColor(ren,120,170,240,180);
        fillCircle(ren,LOGO_CX,LOGO_CY,NUCLEUS_R+2);
        SDL_SetRenderDrawColor(ren,230,242,255,255);
        fillCircle(ren,LOGO_CX,LOGO_CY,NUCLEUS_R);
        SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_BLEND);

        // Divider
        for(int y=24;y<HEADER_H-24;++y){
            float t=(float)(y-24)/(HEADER_H-48);
            float a=std::sin(t*static_cast<float>(M_PI));
            Uint8 av=(Uint8)(a*38);
            SDL_SetRenderDrawColor(ren,av,av,av,255);
            int divX=LOGO_CX+(int)RING_RX+52;
            SDL_RenderDrawPoint(ren,divX,y);
            SDL_RenderDrawPoint(ren,divX+1,y);
        }

        // Title
        {
            int tX=LOGO_CX+(int)RING_RX+68;
            int tY=LOGO_CY-56;
            if(fBig){
                drawText(ren,fBig,"ORBIT-MED",tX+2,tY+2,{0,0,0,255});
                drawText(ren,fBig,"ORBIT-MED",tX,tY,WHITE);
            }
            int tw=fBig?textW(fBig,"ORBIT-MED"):220;
            SDL_SetRenderDrawColor(ren,48,48,48,255);
            SDL_RenderDrawLine(ren,tX,tY+38,tX+tw,tY+38);
            if(fSub){
                drawText(ren,fSub,"Medicine Search & Reference Database",tX,tY+46,{140,170,210,255});
                char cb[80];
                std::snprintf(cb,sizeof(cb),"%d medicines indexed   |   Click any row for details",MEDICINE_COUNT);
                drawText(ren,fSub,cb,tX,tY+62,DGRAY);
            }
            if(fSub){
                int sX=tX+480;
                char rb[64];
                std::snprintf(rb,sizeof(rb),"Showing %d / %d",
                    (int)results.size(),MEDICINE_COUNT);
                drawText(ren,fSub,rb,sX,tY+46,MGRAY);
                std::string sl="Sort: ";
                switch(sortField){
                    case SortField::Name:     sl+="Name";     break;
                    case SortField::Price:    sl+="Price";    break;
                    case SortField::Category: sl+="Category"; break;
                    case SortField::Dosage:   sl+="Dosage";   break;
                }
                sl+=sortAsc?" (A-Z / Low)":"  (Z-A / High)";
                if(!typeFilter.empty()) sl+="   Filter:"+typeFilter;
                drawText(ren,fSub,sl,sX,tY+60,DGRAY);
            }
        }

        // ── Search bar ────────────────────────────────────────────────────────
        SDL_SetRenderDrawColor(ren,28,28,28,255);
        SDL_Rect sb{TABLE_X,SEARCH_Y,WIN_W-TABLE_X*2,SEARCH_H};
        SDL_RenderFillRect(ren,&sb);
        SDL_SetRenderDrawColor(ren,SBRD.r,SBRD.g,SBRD.b,255);
        SDL_RenderDrawRect(ren,&sb);
        if(fMed){
            const char* lbl="SEARCH  ";
            drawText(ren,fMed,lbl,TABLE_X+10,SEARCH_Y+8,DGRAY);
            int lw=textW(fMed,lbl);
            SDL_SetRenderDrawColor(ren,45,45,45,255);
            SDL_RenderDrawLine(ren,TABLE_X+lw+9,SEARCH_Y+6,
                                   TABLE_X+lw+9,SEARCH_Y+SEARCH_H-6);
            if(searchQuery.empty())
                drawText(ren,fMed,"filter by name, category, or use...",
                         TABLE_X+lw+16,SEARCH_Y+8,DGRAY);
            else{
                std::string disp=searchQuery;
                if(std::fmod(blinkT,1.f)<0.5f)disp+="|";
                drawText(ren,fMed,disp,TABLE_X+lw+16,SEARCH_Y+8,WHITE);
            }
        }

        // ── Hint line ─────────────────────────────────────────────────────────
        if(fSub)
            drawText(ren,fSub,
                "F1 Name  F2 Price  F3 Category  F4 Dosage  (press twice to reverse)"
                "   |   F5 All  F6 OTC  F7 Rx   |   Del: clear   |   Click row: details",
                TABLE_X,HINT_Y,DGRAY);

        // ── Table header with sort indicator ──────────────────────────────────
        SDL_SetRenderDrawColor(ren,24,24,24,255);
        SDL_Rect thdr{0,TABLE_Y,WIN_W,ROW_H+1};
        SDL_RenderFillRect(ren,&thdr);
        SDL_SetRenderDrawColor(ren,SEP.r,SEP.g,SEP.b,255);
        SDL_RenderDrawLine(ren,0,TABLE_Y,WIN_W,TABLE_Y);
        SDL_RenderDrawLine(ren,0,TABLE_Y+ROW_H,WIN_W,TABLE_Y+ROW_H);
        if(fSub){
            auto colHdr=[&](const char* lbl,SortField sf,int x){
                std::string s=lbl;
                if(sortField==sf) s+=sortAsc?" ^":" v";
                SDL_Color c=(sortField==sf)?WHITE:ACCENT;
                drawText(ren,fSub,s,x,TABLE_Y+5,c);
            };
            drawText(ren,fSub,"#",TABLE_X,TABLE_Y+5,DGRAY);
            colHdr("NAME",    SortField::Name,    X_NAME);
            colHdr("CATEGORY",SortField::Category,X_CAT);
            drawText(ren,fSub,"USE / INDICATION",X_USE,TABLE_Y+5,ACCENT);
            colHdr("DOSE",    SortField::Dosage,  X_DOS);
            colHdr("PRICE",   SortField::Price,   X_PRI);
            drawText(ren,fSub,"TYPE",X_TYPE,TABLE_Y+5,ACCENT);
        }
        SDL_SetRenderDrawColor(ren,SEP.r,SEP.g,SEP.b,150);
        for(int cx:{X_NAME,X_CAT,X_USE,X_DOS,X_PRI,X_TYPE})
            SDL_RenderDrawLine(ren,cx-4,TABLE_Y+2,cx-4,TABLE_Y+ROW_H-2);

        // ── Table rows ────────────────────────────────────────────────────────
        int tBottom=WIN_H-28;
        SDL_Rect clip{0,TABLE_Y+ROW_H,WIN_W,tBottom-TABLE_Y-ROW_H};
        SDL_RenderSetClipRect(ren,&clip);

        if(results.empty()){
            SDL_RenderSetClipRect(ren,nullptr);
            if(fMed) drawText(ren,fMed,"No medicines found.",TABLE_X,TABLE_Y+ROW_H+12,MGRAY);
        } else {
            int drawn=0;
            for(int vi=scrollOff;vi<(int)results.size();++vi){
                int ry=TABLE_Y+ROW_H+drawn*ROW_H;
                if(ry+ROW_H>tBottom) break;
                const Medicine& m=MEDICINES[results[vi]];
                bool isHover=(vi==hoverRow);

                // Row background
                if(isHover){
                    SDL_SetRenderDrawColor(ren,36,36,36,255);
                } else if(drawn%2==0){
                    SDL_SetRenderDrawColor(ren,ROWALT.r,ROWALT.g,ROWALT.b,255);
                } else {
                    SDL_SetRenderDrawColor(ren,BG.r,BG.g,BG.b,255);
                }
                SDL_Rect rb2{0,ry,WIN_W,ROW_H};
                SDL_RenderFillRect(ren,&rb2);

                // Hover left accent bar
                if(isHover){
                    SDL_SetRenderDrawColor(ren,80,130,220,255);
                    SDL_RenderDrawLine(ren,0,ry,0,ry+ROW_H-1);
                    SDL_RenderDrawLine(ren,1,ry,1,ry+ROW_H-1);
                    SDL_RenderDrawLine(ren,2,ry,2,ry+ROW_H-1);
                }

                SDL_SetRenderDrawColor(ren,SEP.r,SEP.g,SEP.b,50);
                SDL_RenderDrawLine(ren,TABLE_X,ry+ROW_H-1,WIN_W-TABLE_X,ry+ROW_H-1);

                if(fSub){
                    char ib[8];
                    std::snprintf(ib,sizeof(ib),"%d",vi+1);
                    drawText(ren,fSub,ib,TABLE_X,ry+5,DGRAY);
                    SDL_Color nameC=isHover?WHITE:WHITE;
                    drawText(ren,fSub,trunc(m.name,30),  X_NAME,ry+5,nameC);
                    drawText(ren,fSub,trunc(m.category,18),X_CAT,ry+5,catColor(m.category));
                    drawText(ren,fSub,trunc(m.use,42),   X_USE, ry+5,LGRAY);
                    drawText(ren,fSub,trunc(m.dosage,11),X_DOS, ry+5,LGRAY);
                    char pb[16];
                    std::snprintf(pb,sizeof(pb),"$%.2f",m.price);
                    drawText(ren,fSub,pb,X_PRI,ry+5,LGRAY);
                    SDL_Color tc=(std::string(m.type)=="OTC")
                        ?SDL_Color{185,215,185,255}
                        :SDL_Color{215,180,180,255};
                    drawText(ren,fSub,m.type,X_TYPE,ry+5,tc);
                }
                ++drawn;
            }
            SDL_RenderSetClipRect(ren,nullptr);

            // Scrollbar
            if((int)results.size()>maxVis){
                int sbX=WIN_W-TABLE_X-SCROLLBAR_W+2;
                int sbTop=TABLE_Y+ROW_H;
                int sbH=tBottom-sbTop;
                SDL_SetRenderDrawColor(ren,22,22,22,255);
                SDL_Rect tr{sbX,sbTop,SCROLLBAR_W,sbH};
                SDL_RenderFillRect(ren,&tr);
                float ratio=(float)maxVis/results.size();
                float th=std::max(20.f,sbH*ratio);
                float tt=sbTop+(sbH-th)*(maxSc>0?(float)scrollOff/maxSc:0.f);
                SDL_SetRenderDrawColor(ren,72,72,72,255);
                SDL_Rect thumb{sbX+1,(int)tt,SCROLLBAR_W-2,(int)th};
                SDL_RenderFillRect(ren,&thumb);
            }
        }

        // Status bar
        SDL_SetRenderDrawColor(ren,16,16,16,255);
        SDL_Rect stbg{0,WIN_H-26,WIN_W,26};
        SDL_RenderFillRect(ren,&stbg);
        SDL_SetRenderDrawColor(ren,SEP.r,SEP.g,SEP.b,255);
        SDL_RenderDrawLine(ren,0,WIN_H-27,WIN_W,WIN_H-27);
        if(fSub){
            char stbuf[160];
            std::snprintf(stbuf,sizeof(stbuf),
                "  %d of %d results   |   row %d/%d   |   Orbit-Med v4.0   |   ESC to quit",
                (int)results.size(),MEDICINE_COUNT,
                scrollOff+1,std::max(1,(int)results.size()));
            drawText(ren,fSub,stbuf,6,WIN_H-19,DGRAY);
        }

        SDL_RenderPresent(ren);

        // ── Render detail window if open ────────────────────────────────────
        if(detWin&&detRen&&detMed>=0)
            renderDetailWindow(detRen,detMed,fBig,fMed,fSub);
    }

    closeDetail();
    SDL_StopTextInput();
    if(fSub)TTF_CloseFont(fSub);
    if(fMed)TTF_CloseFont(fMed);
    if(fBig)TTF_CloseFont(fBig);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();IMG_Quit();SDL_Quit();
    return 0;
}
