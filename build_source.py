import json

# ── Load existing medicine entries ────────────────────────────────────────────
with open(r'C:/Users/Karson/Desktop/Orbit Projects/Orbit-Medicin/meds_data.txt','r',encoding='utf-8') as f:
    old_meds = f.read().strip()

# ── Additional medicines (~350 more) ─────────────────────────────────────────
extra_meds = """
    { "Epinephrine (EpiPen)", "Emergency", "Anaphylaxis, cardiac arrest", "0.3mg", 650.00f, "Rx" },
    { "Naloxone (Narcan)", "Emergency", "Opioid overdose reversal", "0.4mg", 140.00f, "Rx" },
    { "Atropine", "Emergency", "Bradycardia, organophosphate poisoning", "0.5mg", 18.00f, "Rx" },
    { "Adenosine", "Emergency", "SVT, paroxysmal supraventricular tachycardia", "6mg", 45.00f, "Rx" },
    { "Amiodarone", "Heart", "Ventricular arrhythmias", "200mg", 38.00f, "Rx" },
    { "Sodium Bicarbonate", "Emergency", "Metabolic acidosis, drug toxicity", "50mEq", 12.00f, "Rx" },
    { "Activated Charcoal", "Emergency", "Poisoning, drug overdose", "25g", 22.00f, "Rx" },
    { "Glucagon", "Emergency", "Severe hypoglycemia", "1mg", 180.00f, "Rx" },
    { "Vasopressin", "Emergency", "Septic shock, cardiac arrest", "20units", 95.00f, "Rx" },
    { "Nitroglycerin (Nitrostat)", "Heart", "Angina, acute chest pain", "0.4mg", 28.00f, "Rx" },
    { "Furosemide (Lasix)", "Heart", "Edema, heart failure, hypertension", "40mg", 9.50f, "Rx" },
    { "Spironolactone", "Heart", "Heart failure, hyperaldosteronism", "25mg", 11.00f, "Rx" },
    { "Digoxin (Lanoxin)", "Heart", "Atrial fibrillation, heart failure", "0.125mg", 14.00f, "Rx" },
    { "Clopidogrel (Plavix)", "Heart", "Anti-platelet, prevent clots", "75mg", 22.00f, "Rx" },
    { "Apixaban (Eliquis)", "Blood", "Stroke prevention, DVT/PE", "5mg", 480.00f, "Rx" },
    { "Rivaroxaban (Xarelto)", "Blood", "DVT, PE, stroke prevention", "20mg", 460.00f, "Rx" },
    { "Dabigatran (Pradaxa)", "Blood", "Stroke prevention in A-fib", "150mg", 440.00f, "Rx" },
    { "Heparin", "Blood", "DVT prophylaxis, anticoagulation", "5000u", 32.00f, "Rx" },
    { "Enoxaparin (Lovenox)", "Blood", "DVT prevention and treatment", "40mg", 85.00f, "Rx" },
    { "Ferrous Gluconate", "Blood", "Iron deficiency anemia", "300mg", 7.99f, "OTC" },
    { "Folic Acid", "Vitamin", "Neural tube defect prevention, anemia", "1mg", 5.49f, "OTC" },
    { "Erythropoietin (Epogen)", "Blood", "Anemia from CKD, chemotherapy", "4000u", 320.00f, "Rx" },
    { "Hydroxyurea", "Blood", "Sickle cell disease, polycythemia", "500mg", 68.00f, "Rx" },
    { "Desmopressin (DDAVP)", "Blood", "Hemophilia A, von Willebrand disease", "0.3mcg/kg", 145.00f, "Rx" },
    { "Levothyroxine (Synthroid)", "Thyroid", "Hypothyroidism", "50mcg", 12.00f, "Rx" },
    { "Liothyronine (Cytomel)", "Thyroid", "Hypothyroidism, thyroid cancer", "25mcg", 38.00f, "Rx" },
    { "Methimazole (Tapazole)", "Thyroid", "Hyperthyroidism, Graves disease", "10mg", 22.00f, "Rx" },
    { "Propylthiouracil (PTU)", "Thyroid", "Hyperthyroidism", "50mg", 19.00f, "Rx" },
    { "Potassium Iodide", "Thyroid", "Radiation emergency, thyroid protection", "130mg", 11.00f, "OTC" },
    { "Alendronate (Fosamax)", "Bone", "Osteoporosis prevention and treatment", "70mg", 25.00f, "Rx" },
    { "Risedronate (Actonel)", "Bone", "Osteoporosis, Paget disease", "35mg", 28.00f, "Rx" },
    { "Zoledronic Acid (Reclast)", "Bone", "Osteoporosis, bone metastases", "5mg", 980.00f, "Rx" },
    { "Teriparatide (Forteo)", "Bone", "Severe osteoporosis", "20mcg", 2800.00f, "Rx" },
    { "Denosumab (Prolia)", "Bone", "Osteoporosis, bone metastases", "60mg", 1200.00f, "Rx" },
    { "Calcium Carbonate (Tums)", "Bone", "Calcium supplement, acid relief", "500mg", 6.99f, "OTC" },
    { "Calcium Citrate (Citracal)", "Bone", "Osteoporosis prevention", "630mg", 12.99f, "OTC" },
    { "Raloxifene (Evista)", "Bone", "Osteoporosis in postmenopausal women", "60mg", 48.00f, "Rx" },
    { "Conjugated Estrogen (Premarin)", "Women's Health", "Menopause symptoms, osteoporosis", "0.625mg", 55.00f, "Rx" },
    { "Estradiol (Estrace)", "Women's Health", "Menopause HRT, female hypogonadism", "1mg", 38.00f, "Rx" },
    { "Progesterone (Prometrium)", "Women's Health", "HRT, endometrial protection", "200mg", 62.00f, "Rx" },
    { "Clomiphene (Clomid)", "Women's Health", "Ovulation induction, infertility", "50mg", 42.00f, "Rx" },
    { "Letrozole (Femara)", "Women's Health", "Infertility, breast cancer", "2.5mg", 58.00f, "Rx" },
    { "Methotrexate", "Women's Health", "Ectopic pregnancy, RA, psoriasis", "2.5mg", 35.00f, "Rx" },
    { "Mifepristone (Mifeprex)", "Women's Health", "Medical abortion, Cushing syndrome", "200mg", 95.00f, "Rx" },
    { "Norethindrone", "Women's Health", "Contraception, endometriosis", "0.35mg", 22.00f, "Rx" },
    { "Levonorgestrel (Plan B)", "Women's Health", "Emergency contraception", "1.5mg", 49.99f, "OTC" },
    { "Ethinyl Estradiol/Norgestimate", "Women's Health", "Oral contraceptive", "0.035mg", 35.00f, "Rx" },
    { "Etonogestrel Implant (Nexplanon)", "Women's Health", "Long-term contraception", "68mg", 800.00f, "Rx" },
    { "Dinoprostone (Cervidil)", "Women's Health", "Cervical ripening, labor induction", "10mg", 280.00f, "Rx" },
    { "Oxytocin (Pitocin)", "Women's Health", "Labor induction, postpartum bleeding", "10units", 25.00f, "Rx" },
    { "Sildenafil (Viagra)", "Men's Health", "Erectile dysfunction, pulmonary HTN", "50mg", 65.00f, "Rx" },
    { "Tadalafil (Cialis)", "Men's Health", "Erectile dysfunction, BPH", "10mg", 70.00f, "Rx" },
    { "Vardenafil (Levitra)", "Men's Health", "Erectile dysfunction", "10mg", 68.00f, "Rx" },
    { "Finasteride (Proscar)", "Men's Health", "BPH, male pattern baldness", "5mg", 28.00f, "Rx" },
    { "Dutasteride (Avodart)", "Men's Health", "Benign prostatic hyperplasia", "0.5mg", 32.00f, "Rx" },
    { "Tamsulosin (Flomax)", "Men's Health", "Benign prostatic hyperplasia", "0.4mg", 18.00f, "Rx" },
    { "Testosterone Cypionate", "Men's Health", "Hypogonadism, testosterone deficiency", "200mg/mL", 85.00f, "Rx" },
    { "Testosterone Gel (AndroGel)", "Men's Health", "Hypogonadism", "1.62%", 380.00f, "Rx" },
    { "Clonazepam (Klonopin)", "Mental Health", "Seizures, panic disorder", "1mg", 16.00f, "Rx" },
    { "Lorazepam (Ativan)", "Mental Health", "Anxiety, sedation, seizures", "1mg", 14.00f, "Rx" },
    { "Diazepam (Valium)", "Mental Health", "Anxiety, muscle spasms, seizures", "5mg", 12.00f, "Rx" },
    { "Quetiapine (Seroquel)", "Mental Health", "Schizophrenia, bipolar, depression", "50mg", 28.00f, "Rx" },
    { "Olanzapine (Zyprexa)", "Mental Health", "Schizophrenia, bipolar disorder", "5mg", 32.00f, "Rx" },
    { "Risperidone (Risperdal)", "Mental Health", "Schizophrenia, bipolar disorder", "2mg", 22.00f, "Rx" },
    { "Aripiprazole (Abilify)", "Mental Health", "Schizophrenia, bipolar, depression augmentation", "10mg", 45.00f, "Rx" },
    { "Lithium Carbonate", "Mental Health", "Bipolar disorder mood stabilizer", "300mg", 12.00f, "Rx" },
    { "Valproate (Depakote)", "Mental Health", "Bipolar, epilepsy, migraines", "500mg", 24.00f, "Rx" },
    { "Lamotrigine (Lamictal)", "Mental Health", "Bipolar, epilepsy", "100mg", 22.00f, "Rx" },
    { "Methylphenidate (Ritalin)", "Mental Health", "ADHD, narcolepsy", "10mg", 38.00f, "Rx" },
    { "Amphetamine/Dextroamphetamine (Adderall)", "Mental Health", "ADHD, narcolepsy", "10mg", 42.00f, "Rx" },
    { "Lisdexamfetamine (Vyvanse)", "Mental Health", "ADHD, binge eating disorder", "30mg", 95.00f, "Rx" },
    { "Atomoxetine (Strattera)", "Mental Health", "ADHD non-stimulant", "40mg", 88.00f, "Rx" },
    { "Guanfacine (Intuniv)", "Mental Health", "ADHD, tic disorders", "1mg", 45.00f, "Rx" },
    { "Venlafaxine (Effexor)", "Mental Health", "Depression, anxiety, neuropathy", "75mg", 28.00f, "Rx" },
    { "Duloxetine (Cymbalta)", "Mental Health", "Depression, anxiety, fibromyalgia", "30mg", 32.00f, "Rx" },
    { "Desvenlafaxine (Pristiq)", "Mental Health", "Major depressive disorder", "50mg", 38.00f, "Rx" },
    { "Mirtazapine (Remeron)", "Mental Health", "Depression, insomnia, anxiety", "15mg", 18.00f, "Rx" },
    { "Trazodone (Desyrel)", "Sleep", "Insomnia, depression", "50mg", 14.00f, "Rx" },
    { "Zolpidem (Ambien)", "Sleep", "Insomnia short-term treatment", "5mg", 22.00f, "Rx" },
    { "Eszopiclone (Lunesta)", "Sleep", "Insomnia", "1mg", 28.00f, "Rx" },
    { "Temazepam (Restoril)", "Sleep", "Insomnia, sleep maintenance", "15mg", 16.00f, "Rx" },
    { "Melatonin", "Sleep", "Insomnia, circadian rhythm disorders", "5mg", 8.99f, "OTC" },
    { "Doxylamine (Unisom)", "Sleep", "Insomnia, sleep aid", "25mg", 7.49f, "OTC" },
    { "Ramelteon (Rozerem)", "Sleep", "Insomnia, melatonin receptor agonist", "8mg", 35.00f, "Rx" },
    { "Suvorexant (Belsomra)", "Sleep", "Insomnia, orexin receptor antagonist", "10mg", 285.00f, "Rx" },
    { "Levetiracetam (Keppra)", "Neurological", "Epilepsy, seizure control", "500mg", 28.00f, "Rx" },
    { "Topiramate (Topamax)", "Neurological", "Epilepsy, migraine prevention", "25mg", 32.00f, "Rx" },
    { "Carbamazepine (Tegretol)", "Neurological", "Epilepsy, trigeminal neuralgia", "200mg", 22.00f, "Rx" },
    { "Oxcarbazepine (Trileptal)", "Neurological", "Epilepsy, partial seizures", "300mg", 28.00f, "Rx" },
    { "Phenytoin (Dilantin)", "Neurological", "Epilepsy, seizure prophylaxis", "100mg", 18.00f, "Rx" },
    { "Phenobarbital", "Neurological", "Epilepsy, sedation", "30mg", 12.00f, "Rx" },
    { "Levodopa/Carbidopa (Sinemet)", "Neurological", "Parkinson disease", "25/100mg", 32.00f, "Rx" },
    { "Pramipexole (Mirapex)", "Neurological", "Parkinson disease, RLS", "0.5mg", 28.00f, "Rx" },
    { "Ropinirole (Requip)", "Neurological", "Parkinson disease, restless leg syndrome", "1mg", 24.00f, "Rx" },
    { "Entacapone (Comtan)", "Neurological", "Parkinson disease adjunct", "200mg", 42.00f, "Rx" },
    { "Rasagiline (Azilect)", "Neurological", "Parkinson disease, MAO-B inhibitor", "1mg", 68.00f, "Rx" },
    { "Sumatriptan (Imitrex)", "Neurological", "Migraine, cluster headaches", "50mg", 35.00f, "Rx" },
    { "Rizatriptan (Maxalt)", "Neurological", "Migraine treatment", "10mg", 38.00f, "Rx" },
    { "Eletriptan (Relpax)", "Neurological", "Acute migraine", "40mg", 42.00f, "Rx" },
    { "Propranolol (Inderal)", "Neurological", "Migraine prevention, essential tremor", "40mg", 12.00f, "Rx" },
    { "Amitriptyline (Elavil)", "Neurological", "Neuropathy, migraine prevention, depression", "25mg", 10.00f, "Rx" },
    { "Donepezil (Aricept)", "Neurological", "Alzheimer disease, dementia", "5mg", 28.00f, "Rx" },
    { "Memantine (Namenda)", "Neurological", "Alzheimer disease, moderate-severe", "10mg", 32.00f, "Rx" },
    { "Rivastigmine (Exelon)", "Neurological", "Alzheimer, Parkinson dementia", "3mg", 38.00f, "Rx" },
    { "Interferon Beta-1a (Avonex)", "Neurological", "Multiple sclerosis", "30mcg", 3800.00f, "Rx" },
    { "Glatiramer (Copaxone)", "Neurological", "Multiple sclerosis", "20mg", 5200.00f, "Rx" },
    { "Baclofen", "Muscle Relaxant", "Muscle spasticity, MS", "10mg", 14.00f, "Rx" },
    { "Cyclobenzaprine (Flexeril)", "Muscle Relaxant", "Acute muscle spasm", "5mg", 12.00f, "Rx" },
    { "Methocarbamol (Robaxin)", "Muscle Relaxant", "Muscle spasm, acute pain", "500mg", 14.00f, "Rx" },
    { "Carisoprodol (Soma)", "Muscle Relaxant", "Acute muscle spasm", "350mg", 16.00f, "Rx" },
    { "Tizanidine (Zanaflex)", "Muscle Relaxant", "Spasticity, multiple sclerosis", "4mg", 18.00f, "Rx" },
    { "Diazepam (muscle)", "Muscle Relaxant", "Muscle spasm, spasticity", "5mg", 12.00f, "Rx" },
    { "Dantrolene (Dantrium)", "Muscle Relaxant", "Malignant hyperthermia, spasticity", "25mg", 45.00f, "Rx" },
    { "Botulinum Toxin A (Botox)", "Muscle Relaxant", "Spasticity, dystonia, cosmetic", "100u", 1200.00f, "Rx" },
    { "Oseltamivir (Tamiflu)", "Antiviral", "Influenza A and B treatment", "75mg", 42.00f, "Rx" },
    { "Acyclovir (Zovirax)", "Antiviral", "Herpes simplex, varicella-zoster", "400mg", 18.00f, "Rx" },
    { "Valacyclovir (Valtrex)", "Antiviral", "Herpes, cold sores, shingles", "500mg", 35.00f, "Rx" },
    { "Ganciclovir (Cytovene)", "Antiviral", "CMV infection, immunocompromised", "250mg", 120.00f, "Rx" },
    { "Ribavirin", "Antiviral", "Hepatitis C, RSV", "200mg", 85.00f, "Rx" },
    { "Remdesivir (Veklury)", "Antiviral", "COVID-19, severe viral infections", "100mg", 3120.00f, "Rx" },
    { "Sofosbuvir (Sovaldi)", "Antiviral", "Hepatitis C (HCV)", "400mg", 84000.00f, "Rx" },
    { "Tenofovir (Viread)", "Antiviral", "HIV, hepatitis B", "300mg", 68.00f, "Rx" },
    { "Emtricitabine/Tenofovir (Truvada)", "Antiviral", "HIV PrEP, antiretroviral therapy", "200/300mg", 2000.00f, "Rx" },
    { "Dolutegravir (Tivicay)", "Antiviral", "HIV integrase inhibitor", "50mg", 3800.00f, "Rx" },
    { "Atazanavir (Reyataz)", "Antiviral", "HIV protease inhibitor", "300mg", 2800.00f, "Rx" },
    { "Zanamivir (Relenza)", "Antiviral", "Influenza A and B inhaled treatment", "5mg", 38.00f, "Rx" },
    { "Fluconazole (Diflucan)", "Antifungal", "Candidiasis, cryptococcal meningitis", "150mg", 18.00f, "Rx" },
    { "Itraconazole (Sporanox)", "Antifungal", "Systemic fungal infections", "100mg", 32.00f, "Rx" },
    { "Voriconazole (Vfend)", "Antifungal", "Aspergillosis, serious fungal infections", "200mg", 280.00f, "Rx" },
    { "Amphotericin B (Fungizone)", "Antifungal", "Severe systemic fungal infections", "50mg", 180.00f, "Rx" },
    { "Caspofungin (Cancidas)", "Antifungal", "Invasive candidiasis, aspergillosis", "50mg", 980.00f, "Rx" },
    { "Terbinafine (Lamisil)", "Antifungal", "Tinea infections, onychomycosis", "250mg", 22.00f, "Rx" },
    { "Clotrimazole (Lotrimin)", "Antifungal", "Athlete's foot, ringworm, candidiasis", "1%", 8.99f, "OTC" },
    { "Miconazole (Monistat)", "Antifungal", "Vaginal yeast infection, athlete's foot", "2%", 12.99f, "OTC" },
    { "Nystatin (Mycostatin)", "Antifungal", "Oral and intestinal candidiasis", "500000u", 14.00f, "Rx" },
    { "Salbutamol/Albuterol (Ventolin)", "Respiratory", "Acute bronchospasm, asthma rescue", "90mcg/puff", 32.00f, "Rx" },
    { "Levalbuterol (Xopenex)", "Respiratory", "Bronchospasm, COPD", "0.63mg", 38.00f, "Rx" },
    { "Ipratropium (Atrovent)", "Respiratory", "COPD, bronchospasm", "17mcg/puff", 28.00f, "Rx" },
    { "Tiotropium (Spiriva)", "Respiratory", "COPD maintenance", "18mcg", 380.00f, "Rx" },
    { "Salmeterol (Serevent)", "Respiratory", "Asthma, COPD long-acting bronchodilator", "50mcg", 180.00f, "Rx" },
    { "Formoterol (Foradil)", "Respiratory", "Asthma, COPD long-acting bronchodilator", "12mcg", 165.00f, "Rx" },
    { "Fluticasone (Flovent)", "Respiratory", "Asthma inhaled corticosteroid", "110mcg/puff", 220.00f, "Rx" },
    { "Budesonide (Pulmicort)", "Respiratory", "Asthma, COPD inhaled steroid", "180mcg/puff", 195.00f, "Rx" },
    { "Beclomethasone (QVAR)", "Respiratory", "Asthma prevention, inhaled steroid", "80mcg/puff", 185.00f, "Rx" },
    { "Fluticasone/Salmeterol (Advair)", "Respiratory", "Asthma, COPD combination inhaler", "250/50mcg", 420.00f, "Rx" },
    { "Budesonide/Formoterol (Symbicort)", "Respiratory", "Asthma, COPD combination inhaler", "160/4.5mcg", 390.00f, "Rx" },
    { "Roflumilast (Daliresp)", "Respiratory", "COPD, reduce exacerbations", "500mcg", 380.00f, "Rx" },
    { "Theophylline", "Respiratory", "Asthma, COPD bronchodilator", "200mg", 18.00f, "Rx" },
    { "Guaifenesin (Mucinex)", "Respiratory", "Expectorant, chest congestion", "600mg", 9.99f, "OTC" },
    { "Dextromethorphan (Robitussin DM)", "Respiratory", "Cough suppressant", "15mg", 7.99f, "OTC" },
    { "Benzonatate (Tessalon)", "Respiratory", "Non-opioid cough suppressant", "100mg", 16.00f, "Rx" },
    { "Pseudoephedrine (Sudafed)", "Respiratory", "Nasal congestion decongestant", "60mg", 10.99f, "OTC" },
    { "Oxymetazoline (Afrin)", "Respiratory", "Nasal congestion, max 3-day use", "0.05%", 6.99f, "OTC" },
    { "Fluticasone nasal (Flonase)", "Respiratory", "Allergic rhinitis, nasal corticosteroid", "50mcg/spray", 15.99f, "OTC" },
    { "Omalizumab (Xolair)", "Respiratory", "Severe asthma, chronic hives", "150mg", 1850.00f, "Rx" },
    { "Mepolizumab (Nucala)", "Respiratory", "Severe eosinophilic asthma", "100mg", 3200.00f, "Rx" },
    { "Prednisolone", "Respiratory", "Asthma exacerbation, anti-inflammatory", "5mg", 10.00f, "Rx" },
    { "Prednisone", "Immune", "Inflammation, autoimmune disease, asthma", "10mg", 8.00f, "Rx" },
    { "Methylprednisolone (Solu-Medrol)", "Immune", "Severe inflammation, MS relapse", "40mg", 22.00f, "Rx" },
    { "Dexamethasone", "Immune", "Inflammation, croup, cerebral edema", "0.5mg", 12.00f, "Rx" },
    { "Hydrocortisone", "Immune", "Adrenal insufficiency, inflammation", "20mg", 10.00f, "Rx" },
    { "Tacrolimus (Prograf)", "Immune", "Organ transplant rejection prevention", "1mg", 180.00f, "Rx" },
    { "Cyclosporine (Neoral)", "Immune", "Transplant rejection, RA, psoriasis", "100mg", 240.00f, "Rx" },
    { "Mycophenolate (CellCept)", "Immune", "Organ transplant, lupus nephritis", "500mg", 95.00f, "Rx" },
    { "Azathioprine (Imuran)", "Immune", "Transplant rejection, RA, IBD", "50mg", 38.00f, "Rx" },
    { "Hydroxychloroquine (Plaquenil)", "Immune", "Lupus, rheumatoid arthritis, malaria", "200mg", 22.00f, "Rx" },
    { "Methotrexate (RA)", "Immune", "Rheumatoid arthritis, psoriasis", "7.5mg", 18.00f, "Rx" },
    { "Adalimumab (Humira)", "Immune", "RA, Crohn disease, psoriasis", "40mg", 6200.00f, "Rx" },
    { "Etanercept (Enbrel)", "Immune", "Rheumatoid arthritis, psoriasis", "50mg", 5800.00f, "Rx" },
    { "Infliximab (Remicade)", "Immune", "Crohn disease, RA, UC", "100mg", 1850.00f, "Rx" },
    { "Secukinumab (Cosentyx)", "Immune", "Psoriasis, psoriatic arthritis, AS", "150mg", 5400.00f, "Rx" },
    { "Ustekinumab (Stelara)", "Immune", "Psoriasis, Crohn disease, UC", "45mg", 18000.00f, "Rx" },
    { "Rituximab (Rituxan)", "Immune", "Lymphoma, RA, lupus", "500mg", 8600.00f, "Rx" },
    { "Tretinoin (Retin-A)", "Skin", "Acne, photodamage, wrinkles", "0.025%", 38.00f, "Rx" },
    { "Adapalene (Differin)", "Skin", "Acne, retinoid topical", "0.1%", 16.99f, "OTC" },
    { "Benzoyl Peroxide (Benzac)", "Skin", "Acne, bacterial skin infections", "5%", 9.99f, "OTC" },
    { "Clindamycin topical", "Skin", "Acne topical antibiotic", "1%", 18.00f, "Rx" },
    { "Mupirocin (Bactroban)", "Skin", "Impetigo, MRSA skin infection", "2%", 22.00f, "Rx" },
    { "Hydrocortisone cream", "Skin", "Eczema, contact dermatitis, itching", "1%", 6.99f, "OTC" },
    { "Triamcinolone (Kenalog)", "Skin", "Severe eczema, psoriasis", "0.1%", 14.00f, "Rx" },
    { "Clobetasol (Temovate)", "Skin", "Severe psoriasis, eczema", "0.05%", 22.00f, "Rx" },
    { "Tacrolimus ointment (Protopic)", "Skin", "Atopic dermatitis, eczema", "0.1%", 38.00f, "Rx" },
    { "Calcipotriene (Dovonex)", "Skin", "Psoriasis topical treatment", "0.005%", 32.00f, "Rx" },
    { "Isotretinoin (Accutane)", "Skin", "Severe cystic acne", "20mg", 220.00f, "Rx" },
    { "Acyclovir cream (Zovirax)", "Skin", "Cold sores, herpes labialis", "5%", 14.99f, "OTC" },
    { "Permethrin (Elimite)", "Skin", "Scabies, head lice", "5%", 18.00f, "Rx" },
    { "Ketoconazole shampoo", "Skin", "Dandruff, seborrheic dermatitis", "2%", 16.99f, "OTC" },
    { "Silver Sulfadiazine (Silvadene)", "Skin", "Burn wound infections", "1%", 28.00f, "Rx" },
    { "Brimonidine (Alphagan)", "Eye/ENT", "Glaucoma, ocular hypertension", "0.2%", 35.00f, "Rx" },
    { "Timolol eye drops (Timoptic)", "Eye/ENT", "Glaucoma, ocular hypertension", "0.5%", 22.00f, "Rx" },
    { "Latanoprost (Xalatan)", "Eye/ENT", "Glaucoma, prostaglandin analog", "0.005%", 28.00f, "Rx" },
    { "Prednisolone eye drops", "Eye/ENT", "Eye inflammation, post-surgery", "1%", 18.00f, "Rx" },
    { "Ciprofloxacin eye drops", "Eye/ENT", "Bacterial conjunctivitis, eye infection", "0.3%", 14.00f, "Rx" },
    { "Tobramycin eye drops", "Eye/ENT", "Bacterial eye infection, conjunctivitis", "0.3%", 16.00f, "Rx" },
    { "Artificial Tears (Systane)", "Eye/ENT", "Dry eye syndrome, lubricant", "0.4%", 11.99f, "OTC" },
    { "Ketotifen eye drops (Zaditor)", "Eye/ENT", "Allergic conjunctivitis", "0.025%", 12.99f, "OTC" },
    { "Olopatadine (Patanol)", "Eye/ENT", "Allergic conjunctivitis", "0.1%", 28.00f, "Rx" },
    { "Amoxicillin/Clavulanate (Augmentin)", "Antibiotic", "Resistant bacterial infections", "875/125mg", 28.00f, "Rx" },
    { "Cephalexin (Keflex)", "Antibiotic", "Skin, UTI, respiratory infections", "500mg", 16.00f, "Rx" },
    { "Ceftriaxone (Rocephin)", "Antibiotic", "Severe infections, meningitis, gonorrhea", "1g", 38.00f, "Rx" },
    { "Cefdinir (Omnicef)", "Antibiotic", "Otitis media, sinusitis, pneumonia", "300mg", 22.00f, "Rx" },
    { "Cefuroxime (Ceftin)", "Antibiotic", "Lyme disease, sinusitis, otitis media", "500mg", 26.00f, "Rx" },
    { "Vancomycin", "Antibiotic", "MRSA, C. difficile, severe infections", "125mg", 45.00f, "Rx" },
    { "Linezolid (Zyvox)", "Antibiotic", "MRSA, resistant gram-positive infections", "600mg", 285.00f, "Rx" },
    { "Meropenem (Merrem)", "Antibiotic", "Severe, multidrug-resistant infections", "1g", 180.00f, "Rx" },
    { "Metronidazole (Flagyl)", "Antibiotic", "C. diff, anaerobic, parasitic infections", "500mg", 12.00f, "Rx" },
    { "Trimethoprim/Sulfamethoxazole (Bactrim)", "Antibiotic", "UTI, MRSA, Pneumocystis", "160/800mg", 10.00f, "Rx" },
    { "Nitrofurantoin (Macrobid)", "Antibiotic", "Urinary tract infections", "100mg", 16.00f, "Rx" },
    { "Fosfomycin (Monurol)", "Antibiotic", "Uncomplicated UTI", "3g", 68.00f, "Rx" },
    { "Clindamycin (Cleocin)", "Antibiotic", "Skin, bone, pelvic infections", "300mg", 18.00f, "Rx" },
    { "Tigecycline (Tygacil)", "Antibiotic", "Complicated infections, tetracycline class", "50mg", 120.00f, "Rx" },
    { "Levofloxacin (Levaquin)", "Antibiotic", "Pneumonia, sinusitis, UTI", "500mg", 22.00f, "Rx" },
    { "Moxifloxacin (Avelox)", "Antibiotic", "Community-acquired pneumonia, sinusitis", "400mg", 28.00f, "Rx" },
    { "Empagliflozin (Jardiance)", "Diabetes", "Type 2 diabetes, SGLT-2 inhibitor", "10mg", 520.00f, "Rx" },
    { "Dapagliflozin (Farxiga)", "Diabetes", "Type 2 diabetes, heart failure", "10mg", 500.00f, "Rx" },
    { "Canagliflozin (Invokana)", "Diabetes", "Type 2 diabetes, SGLT-2 inhibitor", "100mg", 490.00f, "Rx" },
    { "Semaglutide (Ozempic)", "Diabetes", "Type 2 diabetes, GLP-1 agonist", "0.5mg", 850.00f, "Rx" },
    { "Semaglutide oral (Rybelsus)", "Diabetes", "Type 2 diabetes, GLP-1 oral", "7mg", 780.00f, "Rx" },
    { "Liraglutide (Victoza)", "Diabetes", "Type 2 diabetes, GLP-1 agonist", "1.2mg", 720.00f, "Rx" },
    { "Dulaglutide (Trulicity)", "Diabetes", "Type 2 diabetes, weekly GLP-1", "0.75mg", 760.00f, "Rx" },
    { "Exenatide (Byetta)", "Diabetes", "Type 2 diabetes, GLP-1 agonist", "5mcg", 480.00f, "Rx" },
    { "Insulin Detemir (Levemir)", "Diabetes", "Type 1 and 2 diabetes basal insulin", "100u/mL", 92.00f, "Rx" },
    { "Insulin Aspart (NovoLog)", "Diabetes", "Type 1 and 2 diabetes rapid-acting", "100u/mL", 88.00f, "Rx" },
    { "Insulin Lispro (Humalog)", "Diabetes", "Type 1 and 2 diabetes rapid-acting", "100u/mL", 86.00f, "Rx" },
    { "Pioglitazone (Actos)", "Diabetes", "Type 2 diabetes, insulin sensitizer", "15mg", 18.00f, "Rx" },
    { "Acarbose (Precose)", "Diabetes", "Type 2 diabetes, alpha-glucosidase inhibitor", "25mg", 22.00f, "Rx" },
    { "Tamoxifen (Nolvadex)", "Oncology", "Breast cancer, SERM", "20mg", 38.00f, "Rx" },
    { "Anastrozole (Arimidex)", "Oncology", "Breast cancer, aromatase inhibitor", "1mg", 45.00f, "Rx" },
    { "Letrozole (Femara-onco)", "Oncology", "Breast cancer aromatase inhibitor", "2.5mg", 52.00f, "Rx" },
    { "Exemestane (Aromasin)", "Oncology", "Breast cancer, aromatase inactivator", "25mg", 48.00f, "Rx" },
    { "Trastuzumab (Herceptin)", "Oncology", "HER2+ breast cancer, gastric cancer", "440mg", 6800.00f, "Rx" },
    { "Imatinib (Gleevec)", "Oncology", "CML, GIST, tyrosine kinase inhibitor", "400mg", 12000.00f, "Rx" },
    { "Erlotinib (Tarceva)", "Oncology", "NSCLC, pancreatic cancer, EGFR inhibitor", "150mg", 8500.00f, "Rx" },
    { "Bevacizumab (Avastin)", "Oncology", "Colorectal, lung, renal cell cancer", "400mg", 4200.00f, "Rx" },
    { "Pembrolizumab (Keytruda)", "Oncology", "Melanoma, lung, bladder cancer PD-1", "200mg", 10500.00f, "Rx" },
    { "Nivolumab (Opdivo)", "Oncology", "Melanoma, lung, kidney cancer PD-1", "240mg", 9800.00f, "Rx" },
    { "Cyclophosphamide (Cytoxan)", "Oncology", "Lymphoma, leukemia, solid tumors", "500mg", 120.00f, "Rx" },
    { "Paclitaxel (Taxol)", "Oncology", "Breast, ovarian, lung, cervical cancer", "30mg/5mL", 380.00f, "Rx" },
    { "Docetaxel (Taxotere)", "Oncology", "Breast, lung, prostate cancer", "80mg", 420.00f, "Rx" },
    { "Doxorubicin (Adriamycin)", "Oncology", "Breast, leukemia, lymphoma", "50mg", 280.00f, "Rx" },
    { "Cisplatin", "Oncology", "Testicular, ovarian, bladder cancer", "50mg", 45.00f, "Rx" },
    { "Carboplatin", "Oncology", "Ovarian, lung, head and neck cancer", "450mg", 85.00f, "Rx" },
    { "Fluorouracil (5-FU)", "Oncology", "Colorectal, breast, skin cancer", "500mg", 32.00f, "Rx" },
    { "Capecitabine (Xeloda)", "Oncology", "Colorectal, breast cancer oral 5-FU", "500mg", 280.00f, "Rx" },
    { "Vincristine (Oncovin)", "Oncology", "Leukemia, lymphoma, solid tumors", "1mg", 48.00f, "Rx" },
    { "Osimertinib (Tagrisso)", "Oncology", "NSCLC EGFR mutation targeted therapy", "80mg", 18000.00f, "Rx" },
    { "Omega-3 Fatty Acids (Fish Oil)", "Vitamin", "Heart health, triglyceride reduction", "1000mg", 14.99f, "OTC" },
    { "Coenzyme Q10 (CoQ10)", "Vitamin", "Antioxidant, heart health, energy", "200mg", 22.99f, "OTC" },
    { "Magnesium Glycinate", "Vitamin", "Muscle cramps, sleep, anxiety", "400mg", 16.99f, "OTC" },
    { "Potassium Chloride", "Vitamin", "Hypokalemia, electrolyte replacement", "20mEq", 8.99f, "OTC" },
    { "Vitamin A (Retinol)", "Vitamin", "Vision, immune function, skin health", "5000IU", 7.99f, "OTC" },
    { "Vitamin E (Tocopherol)", "Vitamin", "Antioxidant, immune function", "400IU", 9.99f, "OTC" },
    { "Vitamin K2 (MK-7)", "Vitamin", "Bone health, calcium regulation", "100mcg", 18.99f, "OTC" },
    { "Biotin (Vitamin B7)", "Vitamin", "Hair, nail growth, metabolism", "5000mcg", 12.99f, "OTC" },
    { "Niacin (Vitamin B3)", "Vitamin", "Cholesterol, pellagra, energy metabolism", "500mg", 10.99f, "OTC" },
    { "Riboflavin (Vitamin B2)", "Vitamin", "Energy metabolism, migraine prevention", "100mg", 8.99f, "OTC" },
    { "Thiamine (Vitamin B1)", "Vitamin", "Beriberi, Wernicke encephalopathy", "100mg", 7.99f, "OTC" },
    { "Pantothenic Acid (B5)", "Vitamin", "Metabolism, wound healing", "500mg", 9.49f, "OTC" },
    { "Selenium", "Vitamin", "Antioxidant, thyroid function", "200mcg", 8.99f, "OTC" },
    { "Chromium Picolinate", "Vitamin", "Blood sugar regulation, diabetes support", "200mcg", 9.99f, "OTC" },
    { "L-Carnitine", "Vitamin", "Energy metabolism, heart health", "500mg", 18.99f, "OTC" },
    { "Alpha Lipoic Acid", "Vitamin", "Antioxidant, neuropathy, diabetes", "600mg", 19.99f, "OTC" },
    { "Probiotics (Lactobacillus)", "Vitamin", "Gut health, diarrhea prevention", "10B CFU", 24.99f, "OTC" },
    { "Psyllium Husk (Metamucil)", "Digestive", "Constipation, cholesterol, IBS", "3.4g", 12.99f, "OTC" },
    { "Polyethylene Glycol (MiraLax)", "Digestive", "Constipation, bowel prep", "17g", 14.99f, "OTC" },
    { "Lactulose", "Digestive", "Constipation, hepatic encephalopathy", "10g/15mL", 16.00f, "Rx" },
    { "Senna (Senokot)", "Digestive", "Constipation, stimulant laxative", "8.6mg", 7.99f, "OTC" },
    { "Docusate (Colace)", "Digestive", "Stool softener, constipation", "100mg", 8.99f, "OTC" },
    { "Simethicone (Gas-X)", "Digestive", "Gas, bloating, flatulence", "125mg", 6.99f, "OTC" },
    { "Calcium Polycarbophil (FiberCon)", "Digestive", "IBS, constipation, bulk laxative", "625mg", 11.99f, "OTC" },
    { "Ondansetron (Zofran)", "Digestive", "Nausea, vomiting, chemotherapy-induced", "4mg", 22.00f, "Rx" },
    { "Metoclopramide (Reglan)", "Digestive", "Gastroparesis, GERD, nausea", "10mg", 12.00f, "Rx" },
    { "Prochlorperazine (Compazine)", "Digestive", "Nausea, vomiting, vertigo", "5mg", 14.00f, "Rx" },
    { "Pantoprazole (Protonix)", "Digestive", "GERD, Zollinger-Ellison syndrome", "40mg", 18.00f, "Rx" },
    { "Esomeprazole (Nexium)", "Digestive", "GERD, H. pylori eradication", "40mg", 22.00f, "OTC" },
    { "Mesalamine (Asacol)", "Digestive", "Ulcerative colitis, Crohn disease", "400mg", 280.00f, "Rx" },
    { "Infliximab (digestive)", "Digestive", "Crohn disease, ulcerative colitis", "100mg", 1850.00f, "Rx" },
    { "Vedolizumab (Entyvio)", "Digestive", "Crohn disease, ulcerative colitis", "300mg", 6800.00f, "Rx" },
    { "Cholestyramine (Questran)", "Digestive", "Hypercholesterolemia, bile acid diarrhea", "4g", 28.00f, "Rx" },
    { "Famotidine (Pepcid)", "Digestive", "Heartburn, GERD, peptic ulcer", "20mg", 8.99f, "OTC" },
    { "Sucralfate (Carafate)", "Digestive", "Peptic ulcer, GERD", "1g", 18.00f, "Rx" },
    { "Rosuvastatin (Crestor)", "Heart", "High cholesterol, LDL reduction", "10mg", 22.00f, "Rx" },
    { "Pravastatin (Pravachol)", "Heart", "High cholesterol, cardiovascular prevention", "40mg", 18.00f, "Rx" },
    { "Simvastatin (Zocor)", "Heart", "High cholesterol, cardiovascular risk", "20mg", 14.00f, "Rx" },
    { "Ezetimibe (Zetia)", "Heart", "High cholesterol, cholesterol absorption", "10mg", 32.00f, "Rx" },
    { "Evolocumab (Repatha)", "Heart", "High cholesterol, PCSK9 inhibitor", "140mg", 580.00f, "Rx" },
    { "Valsartan (Diovan)", "Heart", "Hypertension, heart failure, ARB", "80mg", 22.00f, "Rx" },
    { "Losartan (Cozaar)", "Heart", "Hypertension, diabetic nephropathy", "50mg", 18.00f, "Rx" },
    { "Irbesartan (Avapro)", "Heart", "Hypertension, diabetic nephropathy", "150mg", 22.00f, "Rx" },
    { "Candesartan (Atacand)", "Heart", "Hypertension, heart failure", "8mg", 24.00f, "Rx" },
    { "Enalapril (Vasotec)", "Heart", "Hypertension, heart failure", "10mg", 14.00f, "Rx" },
    { "Ramipril (Altace)", "Heart", "Hypertension, cardiovascular risk reduction", "5mg", 16.00f, "Rx" },
    { "Hydrochlorothiazide (HCTZ)", "Heart", "Hypertension, edema, diuretic", "25mg", 8.00f, "Rx" },
    { "Chlorthalidone", "Heart", "Hypertension, preferred thiazide diuretic", "25mg", 10.00f, "Rx" },
    { "Bumetanide (Bumex)", "Heart", "Edema, heart failure, loop diuretic", "1mg", 12.00f, "Rx" },
    { "Nifedipine (Procardia)", "Heart", "Hypertension, angina, Raynaud", "30mg", 16.00f, "Rx" },
    { "Diltiazem (Cardizem)", "Heart", "Hypertension, angina, atrial fibrillation", "120mg", 18.00f, "Rx" },
    { "Verapamil (Calan)", "Heart", "Hypertension, angina, SVT", "80mg", 14.00f, "Rx" },
    { "Ivermectin", "Antibiotic", "Parasitic infections, scabies, onchocerciasis", "3mg", 18.00f, "Rx" },
    { "Albendazole (Albenza)", "Antibiotic", "Intestinal worms, tapeworm", "200mg", 22.00f, "Rx" },
    { "Mebendazole (Emverm)", "Antibiotic", "Pinworm, roundworm, hookworm", "100mg", 18.00f, "Rx" },
    { "Chloroquine (Aralen)", "Antibiotic", "Malaria prophylaxis and treatment", "250mg", 16.00f, "Rx" },
    { "Hydroxychloroquine (anti-malarial)", "Antibiotic", "Malaria, lupus, rheumatoid arthritis", "200mg", 14.00f, "Rx" },
    { "Atovaquone/Proguanil (Malarone)", "Antibiotic", "Malaria prophylaxis and treatment", "250/100mg", 145.00f, "Rx" },
    { "Pyrimethamine (Daraprim)", "Antibiotic", "Toxoplasmosis, malaria", "25mg", 1200.00f, "Rx" },
    { "Bupropion (Wellbutrin)", "Mental Health", "Depression, seasonal affective, ADHD", "150mg", 28.00f, "Rx" },
    { "Nortriptyline (Pamelor)", "Mental Health", "Depression, neuropathic pain", "25mg", 14.00f, "Rx" },
    { "Imipramine (Tofranil)", "Mental Health", "Depression, enuresis, panic disorder", "25mg", 12.00f, "Rx" },
    { "Clomipramine (Anafranil)", "Mental Health", "OCD, tricyclic antidepressant", "25mg", 16.00f, "Rx" },
    { "Phenelzine (Nardil)", "Mental Health", "Treatment-resistant depression, MAOI", "15mg", 38.00f, "Rx" },
    { "Tranylcypromine (Parnate)", "Mental Health", "Treatment-resistant depression, MAOI", "10mg", 42.00f, "Rx" },
    { "Paliperidone (Invega)", "Mental Health", "Schizophrenia, schizoaffective disorder", "3mg", 45.00f, "Rx" },
    { "Ziprasidone (Geodon)", "Mental Health", "Schizophrenia, bipolar mania", "20mg", 38.00f, "Rx" },
    { "Lurasidone (Latuda)", "Mental Health", "Schizophrenia, bipolar depression", "40mg", 52.00f, "Rx" },
    { "Haloperidol (Haldol)", "Mental Health", "Schizophrenia, Tourette, agitation", "2mg", 12.00f, "Rx" },
    { "Naltrexone (Vivitrol)", "Mental Health", "Alcohol/opioid use disorder", "50mg", 85.00f, "Rx" },
    { "Buprenorphine/Naloxone (Suboxone)", "Mental Health", "Opioid use disorder treatment", "8/2mg", 95.00f, "Rx" },
    { "Acamprosate (Campral)", "Mental Health", "Alcohol use disorder maintenance", "666mg", 45.00f, "Rx" },
    { "Disulfiram (Antabuse)", "Mental Health", "Alcohol use disorder deterrent", "250mg", 38.00f, "Rx" },
    { "Varenicline (Chantix)", "Respiratory", "Smoking cessation", "0.5mg", 220.00f, "Rx" },
    { "Nicotine Patch (Nicoderm)", "Respiratory", "Smoking cessation NRT", "14mg", 38.99f, "OTC" },
    { "Nicotine Gum (Nicorette)", "Respiratory", "Smoking cessation NRT", "2mg", 28.99f, "OTC" },
    { "Orlistat (Alli)", "Digestive", "Obesity, weight management", "60mg", 52.99f, "OTC" },
    { "Phentermine (Adipex)", "Digestive", "Obesity, short-term weight loss", "37.5mg", 32.00f, "Rx" },
    { "Semaglutide (Wegovy)", "Digestive", "Obesity, chronic weight management GLP-1", "2.4mg", 1350.00f, "Rx" },
    { "Tirzepatide (Mounjaro)", "Diabetes", "Type 2 diabetes, weight loss GIP/GLP-1", "5mg", 1050.00f, "Rx" },
    { "Hydroxyzine (Vistaril)", "Allergy", "Anxiety, allergic pruritus, sedation", "25mg", 12.00f, "Rx" },
    { "Promethazine (Phenergan)", "Allergy", "Allergies, nausea, motion sickness", "25mg", 10.00f, "Rx" },
    { "Cyproheptadine (Periactin)", "Allergy", "Allergies, appetite stimulation, migraine", "4mg", 14.00f, "Rx" },
    { "Cromolyn (NasalCrom)", "Allergy", "Allergic rhinitis prevention", "5.2mg/spray", 16.99f, "OTC" },
    { "Ipratropium nasal (Atrovent)", "Allergy", "Rhinorrhea, common cold, rhinitis", "0.03%", 28.00f, "Rx" },
    { "Azelastine nasal (Astelin)", "Allergy", "Allergic rhinitis, nasal antihistamine", "137mcg/spray", 32.00f, "Rx" },
    { "Letermovir (Prevymis)", "Antiviral", "CMV prophylaxis in transplant patients", "480mg", 8800.00f, "Rx" },
    { "Valganciclovir (Valcyte)", "Antiviral", "CMV retinitis, transplant prophylaxis", "450mg", 380.00f, "Rx" },
    { "Baloxavir (Xofluza)", "Antiviral", "Influenza, single-dose treatment", "40mg", 185.00f, "Rx" },
    { "Paxlovid (Nirmatrelvir/Ritonavir)", "Antiviral", "COVID-19 mild to moderate treatment", "300/100mg", 1390.00f, "Rx" },
    { "Molnupiravir (Lagevrio)", "Antiviral", "COVID-19 treatment", "200mg", 700.00f, "Rx" },
"""

# ── Complete C++ source ───────────────────────────────────────────────────────
cpp_header = '''// ============================================================================
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
'''

cpp_footer = '''};
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
    if(c=="women\'s health") return {228,210,222,255};
    if(c=="men\'s health")   return {215,225,228,255};
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
'''

# ── ~400 more medicines ───────────────────────────────────────────────────────
extra_meds2 = """
    { "Semaglutide (Ozempic)", "Diabetes", "Type 2 diabetes, weight management", "0.5-2mg/wk", 850.00f, "Rx" },
    { "Tirzepatide (Mounjaro)", "Diabetes", "Type 2 diabetes, GLP-1/GIP dual agonist", "5-15mg/wk", 1000.00f, "Rx" },
    { "Liraglutide (Victoza)", "Diabetes", "Type 2 diabetes, cardiovascular risk reduction", "1.2mg", 680.00f, "Rx" },
    { "Dulaglutide (Trulicity)", "Diabetes", "Type 2 diabetes, once-weekly GLP-1", "0.75mg/wk", 720.00f, "Rx" },
    { "Exenatide (Byetta)", "Diabetes", "Type 2 diabetes adjunct therapy", "5mcg", 400.00f, "Rx" },
    { "Canagliflozin (Invokana)", "Diabetes", "Type 2 diabetes, SGLT2 inhibitor", "100mg", 380.00f, "Rx" },
    { "Dapagliflozin (Farxiga)", "Diabetes", "Type 2 diabetes, heart failure, CKD", "10mg", 390.00f, "Rx" },
    { "Empagliflozin (Jardiance)", "Diabetes", "Type 2 diabetes, heart failure", "10mg", 420.00f, "Rx" },
    { "Ertugliflozin (Steglatro)", "Diabetes", "Type 2 diabetes, SGLT2 inhibitor", "5mg", 370.00f, "Rx" },
    { "Sitagliptin (Januvia)", "Diabetes", "Type 2 diabetes, DPP-4 inhibitor", "100mg", 340.00f, "Rx" },
    { "Saxagliptin (Onglyza)", "Diabetes", "Type 2 diabetes, DPP-4 inhibitor", "5mg", 320.00f, "Rx" },
    { "Linagliptin (Tradjenta)", "Diabetes", "Type 2 diabetes, DPP-4 inhibitor", "5mg", 310.00f, "Rx" },
    { "Alogliptin (Nesina)", "Diabetes", "Type 2 diabetes, DPP-4 inhibitor", "25mg", 290.00f, "Rx" },
    { "Pioglitazone (Actos)", "Diabetes", "Type 2 diabetes, insulin sensitizer", "15mg", 28.00f, "Rx" },
    { "Repaglinide (Prandin)", "Diabetes", "Type 2 diabetes, meglitinide", "0.5mg", 45.00f, "Rx" },
    { "Nateglinide (Starlix)", "Diabetes", "Type 2 diabetes, meglitinide", "60mg", 38.00f, "Rx" },
    { "Acarbose (Precose)", "Diabetes", "Type 2 diabetes, alpha-glucosidase inhibitor", "25mg", 22.00f, "Rx" },
    { "Pramlintide (Symlin)", "Diabetes", "Type 1 and 2 diabetes adjunct", "60mcg", 180.00f, "Rx" },
    { "Insulin Lispro (Humalog)", "Diabetes", "Rapid-acting insulin for meals", "100u/mL", 95.00f, "Rx" },
    { "Insulin Aspart (NovoLog)", "Diabetes", "Rapid-acting mealtime insulin", "100u/mL", 98.00f, "Rx" },
    { "Insulin Glulisine (Apidra)", "Diabetes", "Rapid-acting insulin analog", "100u/mL", 92.00f, "Rx" },
    { "Insulin Glargine (Lantus)", "Diabetes", "Long-acting basal insulin", "100u/mL", 185.00f, "Rx" },
    { "Insulin Detemir (Levemir)", "Diabetes", "Long-acting basal insulin analog", "100u/mL", 170.00f, "Rx" },
    { "Insulin Degludec (Tresiba)", "Diabetes", "Ultra-long-acting basal insulin", "100u/mL", 210.00f, "Rx" },
    { "Insulin NPH", "Diabetes", "Intermediate-acting insulin", "100u/mL", 55.00f, "Rx" },
    { "Sacubitril/Valsartan (Entresto)", "Heart", "Heart failure with reduced ejection fraction", "24/26mg", 480.00f, "Rx" },
    { "Ivabradine (Corlanor)", "Heart", "Heart failure, reduces heart rate", "5mg", 280.00f, "Rx" },
    { "Eplerenone (Inspra)", "Heart", "Heart failure, hypertension, mineralocorticoid antagonist", "25mg", 95.00f, "Rx" },
    { "Vericiguat (Verquvo)", "Heart", "Chronic heart failure, sGC stimulator", "2.5mg", 580.00f, "Rx" },
    { "Isosorbide Mononitrate", "Heart", "Stable angina, prophylaxis", "30mg", 18.00f, "Rx" },
    { "Ranolazine (Ranexa)", "Heart", "Chronic angina, anti-ischemic", "500mg", 220.00f, "Rx" },
    { "Dronedarone (Multaq)", "Heart", "Atrial fibrillation rhythm control", "400mg", 340.00f, "Rx" },
    { "Flecainide (Tambocor)", "Heart", "Atrial fibrillation, arrhythmias", "50mg", 45.00f, "Rx" },
    { "Propafenone (Rhythmol)", "Heart", "Atrial fibrillation, ventricular arrhythmias", "150mg", 52.00f, "Rx" },
    { "Sotalol (Betapace)", "Heart", "Ventricular arrhythmias, atrial fibrillation", "80mg", 38.00f, "Rx" },
    { "Dofetilide (Tikosyn)", "Heart", "Atrial fibrillation maintenance", "125mcg", 180.00f, "Rx" },
    { "Alirocumab (Praluent)", "Blood", "High LDL cholesterol, PCSK9 inhibitor", "75mg", 580.00f, "Rx" },
    { "Evolocumab (Repatha)", "Blood", "High LDL, PCSK9 inhibitor, cardiovascular risk", "140mg", 560.00f, "Rx" },
    { "Inclisiran (Leqvio)", "Blood", "High LDL, siRNA PCSK9 inhibitor, twice yearly", "284mg", 3200.00f, "Rx" },
    { "Bempedoic Acid (Nexletol)", "Blood", "High LDL, ATP-citrate lyase inhibitor", "180mg", 280.00f, "Rx" },
    { "Icosapent Ethyl (Vascepa)", "Blood", "Hypertriglyceridemia, cardiovascular risk", "4g/day", 420.00f, "Rx" },
    { "Lomitapide (Juxtapid)", "Blood", "Familial hypercholesterolemia, MTP inhibitor", "5mg", 18000.00f, "Rx" },
    { "Colchicine (Colcrys)", "Pain Relief", "Gout flares, pericarditis, FMF", "0.6mg", 45.00f, "Rx" },
    { "Febuxostat (Uloric)", "Pain Relief", "Gout, chronic hyperuricemia", "40mg", 85.00f, "Rx" },
    { "Pegloticase (Krystexxa)", "Pain Relief", "Refractory gout, urate-lowering", "8mg", 6500.00f, "Rx" },
    { "Probenecid", "Pain Relief", "Gout, uricosuric agent", "500mg", 18.00f, "Rx" },
    { "Ketorolac (Toradol)", "Pain Relief", "Short-term acute pain, NSAID injection", "30mg", 12.00f, "Rx" },
    { "Diclofenac (Voltaren)", "Pain Relief", "Arthritis, acute pain, NSAID", "50mg", 22.00f, "Rx" },
    { "Indomethacin (Indocin)", "Pain Relief", "Gout, arthritis, acute pain", "25mg", 15.00f, "Rx" },
    { "Piroxicam (Feldene)", "Pain Relief", "Arthritis, NSAID once daily", "20mg", 18.00f, "Rx" },
    { "Etodolac", "Pain Relief", "Osteoarthritis, rheumatoid arthritis", "400mg", 20.00f, "Rx" },
    { "Ketoprofen", "Pain Relief", "Arthritis, pain, menstrual cramps", "75mg", 16.00f, "Rx" },
    { "Naproxen Sodium (Aleve)", "Pain Relief", "Pain, fever, inflammation", "220mg", 8.99f, "OTC" },
    { "Tapentadol (Nucynta)", "Pain Relief", "Moderate to severe acute pain", "50mg", 85.00f, "Rx" },
    { "Hydromorphone (Dilaudid)", "Pain Relief", "Severe pain, opioid analgesic", "2mg", 28.00f, "Rx" },
    { "Oxymorphone (Opana)", "Pain Relief", "Severe chronic pain, opioid", "5mg", 95.00f, "Rx" },
    { "Methadone (Dolophine)", "Pain Relief", "Opioid use disorder, chronic pain", "5mg", 12.00f, "Rx" },
    { "Naltrexone (Vivitrol)", "Mental Health", "Opioid and alcohol use disorder", "50mg", 95.00f, "Rx" },
    { "Ketamine (Ketalar)", "Neurological", "Anesthesia, treatment-resistant depression", "500mg", 280.00f, "Rx" },
    { "Esketamine (Spravato)", "Mental Health", "Treatment-resistant depression, nasal spray", "56mg", 800.00f, "Rx" },
    { "Cariprazine (Vraylar)", "Mental Health", "Schizophrenia, bipolar disorder", "1.5mg", 110.00f, "Rx" },
    { "Brexpiprazole (Rexulti)", "Mental Health", "Schizophrenia, major depressive disorder adjunct", "1mg", 125.00f, "Rx" },
    { "Lumateperone (Caplyta)", "Mental Health", "Schizophrenia, bipolar depression", "42mg", 140.00f, "Rx" },
    { "Paliperidone (Invega)", "Mental Health", "Schizophrenia, schizoaffective disorder", "3mg", 95.00f, "Rx" },
    { "Asenapine (Saphris)", "Mental Health", "Schizophrenia, bipolar I disorder", "5mg", 88.00f, "Rx" },
    { "Ziprasidone (Geodon)", "Mental Health", "Schizophrenia, bipolar disorder", "20mg", 75.00f, "Rx" },
    { "Clozapine (Clozaril)", "Mental Health", "Treatment-resistant schizophrenia", "25mg", 45.00f, "Rx" },
    { "Haloperidol (Haldol)", "Mental Health", "Schizophrenia, acute agitation, Tourette", "2mg", 12.00f, "Rx" },
    { "Perphenazine", "Mental Health", "Schizophrenia, nausea", "4mg", 22.00f, "Rx" },
    { "Vilazodone (Viibryd)", "Mental Health", "Major depressive disorder, SSRI+5-HT1A", "10mg", 95.00f, "Rx" },
    { "Vortioxetine (Trintellix)", "Mental Health", "Major depressive disorder", "5mg", 120.00f, "Rx" },
    { "Levomilnacipran (Fetzima)", "Mental Health", "Major depressive disorder, SNRI", "20mg", 110.00f, "Rx" },
    { "Nefazodone", "Mental Health", "Depression, anxiety", "100mg", 28.00f, "Rx" },
    { "Phenelzine (Nardil)", "Mental Health", "Depression, MAOI", "15mg", 38.00f, "Rx" },
    { "Tranylcypromine (Parnate)", "Mental Health", "Depression, MAOI", "10mg", 35.00f, "Rx" },
    { "Pimavanserin (Nuplazid)", "Mental Health", "Parkinson's disease psychosis", "34mg", 2800.00f, "Rx" },
    { "Brexanolone (Zulresso)", "Mental Health", "Postpartum depression, GABA modulator", "60hr infusion", 34000.00f, "Rx" },
    { "Zuranolone (Zurzuvae)", "Mental Health", "Major depressive disorder, postpartum", "50mg", 720.00f, "Rx" },
    { "Erenumab (Aimovig)", "Neurological", "Migraine prevention, CGRP receptor antagonist", "70mg/mo", 580.00f, "Rx" },
    { "Fremanezumab (Ajovy)", "Neurological", "Migraine prevention, CGRP monoclonal", "225mg/mo", 590.00f, "Rx" },
    { "Galcanezumab (Emgality)", "Neurological", "Migraine prevention, cluster headache", "120mg/mo", 575.00f, "Rx" },
    { "Eptinezumab (Vyepti)", "Neurological", "Migraine prevention, IV CGRP antibody", "100mg qrtly", 1200.00f, "Rx" },
    { "Ubrogepant (Ubrelvy)", "Neurological", "Acute migraine, CGRP receptor antagonist", "50mg", 75.00f, "Rx" },
    { "Rimegepant (Nurtec)", "Neurological", "Acute migraine and prevention, CGRP antagonist", "75mg", 80.00f, "Rx" },
    { "Lasmiditan (Reyvow)", "Neurological", "Acute migraine, 5-HT1F agonist", "50mg", 68.00f, "Rx" },
    { "Sumatriptan (Imitrex)", "Neurological", "Acute migraine, triptan", "50mg", 22.00f, "Rx" },
    { "Rizatriptan (Maxalt)", "Neurological", "Acute migraine", "10mg", 28.00f, "Rx" },
    { "Eletriptan (Relpax)", "Neurological", "Acute migraine", "20mg", 32.00f, "Rx" },
    { "Frovatriptan (Frova)", "Neurological", "Acute migraine, menstrual migraine", "2.5mg", 38.00f, "Rx" },
    { "Almotriptan (Axert)", "Neurological", "Acute migraine", "6.25mg", 30.00f, "Rx" },
    { "Naratriptan (Amerge)", "Neurological", "Acute migraine, gentler onset", "1mg", 28.00f, "Rx" },
    { "Zolmitriptan (Zomig)", "Neurological", "Acute migraine, nasal and oral", "2.5mg", 30.00f, "Rx" },
    { "Pramipexole (Mirapex)", "Neurological", "Parkinson's disease, restless legs syndrome", "0.125mg", 22.00f, "Rx" },
    { "Ropinirole (Requip)", "Neurological", "Parkinson's disease, restless legs", "0.25mg", 18.00f, "Rx" },
    { "Rotigotine (Neupro)", "Neurological", "Parkinson's disease, RLS, transdermal patch", "2mg", 240.00f, "Rx" },
    { "Rasagiline (Azilect)", "Neurological", "Parkinson's disease, MAO-B inhibitor", "1mg", 180.00f, "Rx" },
    { "Safinamide (Xadago)", "Neurological", "Parkinson's disease adjunct, MAO-B inhibitor", "50mg", 520.00f, "Rx" },
    { "Entacapone (Comtan)", "Neurological", "Parkinson's disease, COMT inhibitor adjunct", "200mg", 85.00f, "Rx" },
    { "Opicapone (Ongentys)", "Neurological", "Parkinson's disease, COMT inhibitor", "50mg", 580.00f, "Rx" },
    { "Amantadine (Gocovri)", "Neurological", "Parkinson's dyskinesia, influenza", "100mg", 38.00f, "Rx" },
    { "Deutetrabenazine (Austedo)", "Neurological", "Tardive dyskinesia, Huntington chorea", "6mg", 2800.00f, "Rx" },
    { "Valbenazine (Ingrezza)", "Neurological", "Tardive dyskinesia, VMAT2 inhibitor", "40mg", 3200.00f, "Rx" },
    { "Riluzole (Rilutek)", "Neurological", "ALS, glutamate blocker", "50mg", 380.00f, "Rx" },
    { "Edaravone (Radicava)", "Neurological", "ALS, free radical scavenger", "60mg", 15000.00f, "Rx" },
    { "Nusinersen (Spinraza)", "Neurological", "Spinal muscular atrophy, antisense oligonucleotide", "12mg", 125000.00f, "Rx" },
    { "Donepezil (Aricept)", "Neurological", "Alzheimer's disease, cholinesterase inhibitor", "5mg", 22.00f, "Rx" },
    { "Rivastigmine (Exelon)", "Neurological", "Alzheimer's and Parkinson's dementia", "1.5mg", 35.00f, "Rx" },
    { "Galantamine (Razadyne)", "Neurological", "Alzheimer's disease, AChE inhibitor", "4mg", 28.00f, "Rx" },
    { "Memantine (Namenda)", "Neurological", "Moderate-severe Alzheimer's, NMDA antagonist", "5mg", 38.00f, "Rx" },
    { "Lecanemab (Leqembi)", "Neurological", "Early Alzheimer's, amyloid antibody", "10mg/kg biweekly", 26500.00f, "Rx" },
    { "Adalimumab (Humira)", "Immune", "RA, Crohn's, psoriasis, anti-TNF biologic", "40mg/2wk", 2800.00f, "Rx" },
    { "Etanercept (Enbrel)", "Immune", "RA, psoriatic arthritis, anti-TNF", "50mg/wk", 2200.00f, "Rx" },
    { "Infliximab (Remicade)", "Immune", "RA, Crohn's, UC, anti-TNF infusion", "5mg/kg", 1800.00f, "Rx" },
    { "Certolizumab (Cimzia)", "Immune", "RA, Crohn's, anti-TNF PEGylated", "200mg", 2400.00f, "Rx" },
    { "Golimumab (Simponi)", "Immune", "RA, psoriatic arthritis, anti-TNF monthly", "50mg/mo", 2600.00f, "Rx" },
    { "Abatacept (Orencia)", "Immune", "RA, psoriatic arthritis, T-cell costimulation blocker", "125mg/wk", 2400.00f, "Rx" },
    { "Tocilizumab (Actemra)", "Immune", "RA, cytokine release syndrome, IL-6 inhibitor", "162mg/wk", 1800.00f, "Rx" },
    { "Sarilumab (Kevzara)", "Immune", "Rheumatoid arthritis, IL-6 receptor antagonist", "200mg/2wk", 1900.00f, "Rx" },
    { "Secukinumab (Cosentyx)", "Immune", "Psoriasis, psoriatic arthritis, AS, IL-17A blocker", "150mg/mo", 3200.00f, "Rx" },
    { "Ixekizumab (Taltz)", "Immune", "Psoriasis, psoriatic arthritis, IL-17A inhibitor", "80mg/mo", 3400.00f, "Rx" },
    { "Bimekizumab (Bimzelx)", "Immune", "Plaque psoriasis, IL-17A and F inhibitor", "320mg", 5200.00f, "Rx" },
    { "Guselkumab (Tremfya)", "Immune", "Psoriasis, psoriatic arthritis, IL-23 blocker", "100mg q8wk", 3800.00f, "Rx" },
    { "Risankizumab (Skyrizi)", "Immune", "Psoriasis, psoriatic arthritis, Crohn's, IL-23 p19", "150mg q12wk", 4200.00f, "Rx" },
    { "Ustekinumab (Stelara)", "Immune", "Psoriasis, Crohn's, UC, IL-12/23 blocker", "45-90mg", 3600.00f, "Rx" },
    { "Tildrakizumab (Ilumya)", "Immune", "Plaque psoriasis, IL-23 inhibitor", "100mg q12wk", 3500.00f, "Rx" },
    { "Baricitinib (Olumiant)", "Immune", "RA, alopecia areata, COVID, JAK inhibitor", "2mg", 280.00f, "Rx" },
    { "Tofacitinib (Xeljanz)", "Immune", "RA, psoriatic arthritis, UC, JAK inhibitor", "5mg", 260.00f, "Rx" },
    { "Upadacitinib (Rinvoq)", "Immune", "RA, psoriatic arthritis, atopic dermatitis, JAK1", "15mg", 320.00f, "Rx" },
    { "Apremilast (Otezla)", "Immune", "Psoriatic arthritis, plaque psoriasis, PDE4 inhibitor", "30mg", 280.00f, "Rx" },
    { "Belimumab (Benlysta)", "Immune", "Systemic lupus erythematosus, BLyS blocker", "200mg/wk", 2400.00f, "Rx" },
    { "Anifrolumab (Saphnelo)", "Immune", "Systemic lupus erythematosus, type I IFN blocker", "300mg q4wk", 3800.00f, "Rx" },
    { "Dupilumab (Dupixent)", "Immune", "Atopic dermatitis, asthma, CRS, IL-4/13 blocker", "300mg/2wk", 3200.00f, "Rx" },
    { "Mepolizumab (Nucala)", "Respiratory", "Severe eosinophilic asthma, IL-5 blocker", "100mg/mo", 2800.00f, "Rx" },
    { "Benralizumab (Fasenra)", "Respiratory", "Severe eosinophilic asthma, IL-5Ra antibody", "30mg q8wk", 3000.00f, "Rx" },
    { "Tezepelumab (Tezspire)", "Respiratory", "Severe asthma, TSLP blocker", "210mg/mo", 3200.00f, "Rx" },
    { "Omalizumab (Xolair)", "Respiratory", "Allergic asthma, chronic urticaria, anti-IgE", "150-375mg q2-4wk", 2800.00f, "Rx" },
    { "Roflumilast (Daliresp)", "Respiratory", "COPD exacerbation prevention, PDE4 inhibitor", "500mcg", 220.00f, "Rx" },
    { "Pirfenidone (Esbriet)", "Respiratory", "Idiopathic pulmonary fibrosis", "801mg", 850.00f, "Rx" },
    { "Nintedanib (Ofev)", "Respiratory", "Idiopathic pulmonary fibrosis, ILD", "150mg", 980.00f, "Rx" },
    { "Ivacaftor (Kalydeco)", "Respiratory", "Cystic fibrosis, CFTR potentiator", "150mg", 22000.00f, "Rx" },
    { "Elexacaftor/Tezacaftor/Ivacaftor (Trikafta)", "Respiratory", "Cystic fibrosis, F508del mutation", "2+1 tab combo", 28000.00f, "Rx" },
    { "Treprostinil (Remodulin)", "Heart", "Pulmonary arterial hypertension", "1.25ng/kg/min", 8500.00f, "Rx" },
    { "Bosentan (Tracleer)", "Heart", "Pulmonary arterial hypertension, ET receptor blocker", "62.5mg", 3200.00f, "Rx" },
    { "Ambrisentan (Letairis)", "Heart", "Pulmonary arterial hypertension, ERA", "5mg", 3800.00f, "Rx" },
    { "Riociguat (Adempas)", "Heart", "Pulmonary hypertension, sGC stimulator", "1mg", 2200.00f, "Rx" },
    { "Selexipag (Uptravi)", "Heart", "Pulmonary arterial hypertension, prostacyclin receptor", "200mcg", 5200.00f, "Rx" },
    { "Imatinib (Gleevec)", "Oncology", "CML, GIST, BCR-ABL TKI", "400mg", 580.00f, "Rx" },
    { "Dasatinib (Sprycel)", "Oncology", "CML, Philadelphia+ ALL, BCR-ABL TKI", "100mg", 880.00f, "Rx" },
    { "Nilotinib (Tasigna)", "Oncology", "CML, second-line BCR-ABL TKI", "300mg", 950.00f, "Rx" },
    { "Ponatinib (Iclusig)", "Oncology", "CML, T315I mutation, BCR-ABL TKI", "45mg", 18000.00f, "Rx" },
    { "Erlotinib (Tarceva)", "Oncology", "NSCLC, pancreatic cancer, EGFR TKI", "150mg", 480.00f, "Rx" },
    { "Gefitinib (Iressa)", "Oncology", "NSCLC, EGFR mutation, TKI", "250mg", 520.00f, "Rx" },
    { "Osimertinib (Tagrisso)", "Oncology", "NSCLC, EGFR T790M, third-gen TKI", "80mg", 6200.00f, "Rx" },
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
    { "Pembrolizumab (Keytruda)", "Oncology", "Melanoma, NSCLC, multiple cancers, PD-1 blocker", "200mg q3wk", 10500.00f, "Rx" },
    { "Nivolumab (Opdivo)", "Oncology", "Melanoma, NSCLC, RCC, PD-1 checkpoint inhibitor", "240mg q2wk", 9800.00f, "Rx" },
    { "Atezolizumab (Tecentriq)", "Oncology", "NSCLC, urothelial, triple-neg BC, PD-L1 blocker", "1200mg q3wk", 9500.00f, "Rx" },
    { "Durvalumab (Imfinzi)", "Oncology", "NSCLC, biliary cancer, PD-L1 inhibitor", "1500mg q4wk", 9200.00f, "Rx" },
    { "Ipilimumab (Yervoy)", "Oncology", "Melanoma, RCC, CTLA-4 checkpoint inhibitor", "3mg/kg", 28000.00f, "Rx" },
    { "Trastuzumab (Herceptin)", "Oncology", "HER2+ breast cancer, gastric cancer, monoclonal Ab", "4mg/kg load", 2800.00f, "Rx" },
    { "Pertuzumab (Perjeta)", "Oncology", "HER2+ breast cancer, HER2 dimerization inhibitor", "840mg load", 3400.00f, "Rx" },
    { "Bevacizumab (Avastin)", "Oncology", "Colorectal, lung, glioblastoma, anti-VEGF", "5-15mg/kg", 1800.00f, "Rx" },
    { "Cetuximab (Erbitux)", "Oncology", "Colorectal, head/neck cancer, EGFR antibody", "400mg/m2 load", 2800.00f, "Rx" },
    { "Rituximab (Rituxan)", "Oncology", "B-cell lymphoma, CLL, RA, CD20 antibody", "375mg/m2", 2200.00f, "Rx" },
    { "Obinutuzumab (Gazyva)", "Oncology", "CLL, follicular lymphoma, anti-CD20", "1000mg", 3200.00f, "Rx" },
    { "Palbociclib (Ibrance)", "Oncology", "HR+/HER2- breast cancer, CDK4/6 inhibitor", "125mg", 4800.00f, "Rx" },
    { "Ribociclib (Kisqali)", "Oncology", "HR+ breast cancer, CDK4/6 inhibitor", "600mg", 4500.00f, "Rx" },
    { "Abemaciclib (Verzenio)", "Oncology", "HR+ breast cancer, CDK4/6 inhibitor continuous", "150mg", 4200.00f, "Rx" },
    { "Olaparib (Lynparza)", "Oncology", "BRCA+ breast, ovarian, pancreatic cancer, PARP inhibitor", "300mg", 5800.00f, "Rx" },
    { "Niraparib (Zejula)", "Oncology", "Ovarian cancer maintenance, PARP inhibitor", "300mg", 5200.00f, "Rx" },
    { "Vemurafenib (Zelboraf)", "Oncology", "BRAF V600E melanoma, BRAF inhibitor", "960mg", 4800.00f, "Rx" },
    { "Dabrafenib (Tafinlar)", "Oncology", "BRAF V600 melanoma, NSCLC, BRAF inhibitor", "150mg", 5200.00f, "Rx" },
    { "Trametinib (Mekinist)", "Oncology", "BRAF-mutant melanoma, MEK inhibitor", "2mg", 4500.00f, "Rx" },
    { "Pemetrexed (Alimta)", "Oncology", "NSCLC, mesothelioma, antifolate chemotherapy", "500mg/m2", 1200.00f, "Rx" },
    { "Gemcitabine (Gemzar)", "Oncology", "NSCLC, pancreatic, bladder, breast cancer", "1000mg/m2", 850.00f, "Rx" },
    { "Capecitabine (Xeloda)", "Oncology", "Colorectal, breast cancer, oral 5-FU prodrug", "1250mg/m2", 680.00f, "Rx" },
    { "Irinotecan (Camptosar)", "Oncology", "Colorectal cancer, topoisomerase I inhibitor", "125mg/m2", 420.00f, "Rx" },
    { "Topotecan (Hycamtin)", "Oncology", "Ovarian cancer, SCLC, topoisomerase I inhibitor", "1.5mg/m2", 580.00f, "Rx" },
    { "Etoposide (VP-16)", "Oncology", "Lung, testicular cancer, topoisomerase II inhibitor", "50mg", 45.00f, "Rx" },
    { "Vinorelbine (Navelbine)", "Oncology", "NSCLC, breast cancer, vinca alkaloid", "25mg/m2", 380.00f, "Rx" },
    { "Cabazitaxel (Jevtana)", "Oncology", "Prostate cancer, taxane chemotherapy", "25mg/m2", 5800.00f, "Rx" },
    { "Enzalutamide (Xtandi)", "Oncology", "Metastatic prostate cancer, AR inhibitor", "160mg", 8200.00f, "Rx" },
    { "Abiraterone (Zytiga)", "Oncology", "Prostate cancer, CYP17 inhibitor", "1000mg", 3200.00f, "Rx" },
    { "Darolutamide (Nubeqa)", "Oncology", "Non-metastatic prostate cancer, AR antagonist", "600mg", 7800.00f, "Rx" },
    { "Tamoxifen (Nolvadex)", "Oncology", "ER+ breast cancer, SERM", "20mg", 15.00f, "Rx" },
    { "Exemestane (Aromasin)", "Oncology", "Postmenopausal ER+ breast cancer, aromatase inhibitor", "25mg", 45.00f, "Rx" },
    { "Anastrozole (Arimidex)", "Oncology", "Postmenopausal ER+ breast cancer, aromatase inhibitor", "1mg", 35.00f, "Rx" },
    { "Letrozole (Femara)", "Oncology", "Breast cancer, aromatase inhibitor", "2.5mg", 42.00f, "Rx" },
    { "Baloxavir (Xofluza)", "Antiviral", "Influenza, cap-dependent endonuclease inhibitor", "40mg", 95.00f, "Rx" },
    { "Oseltamivir (Tamiflu)", "Antiviral", "Influenza treatment and prophylaxis", "75mg", 55.00f, "Rx" },
    { "Zanamivir (Relenza)", "Antiviral", "Influenza, inhaled neuraminidase inhibitor", "10mg", 45.00f, "Rx" },
    { "Acyclovir (Zovirax)", "Antiviral", "HSV, VZV, EBV infections, nucleoside analog", "400mg", 12.00f, "Rx" },
    { "Valacyclovir (Valtrex)", "Antiviral", "HSV, VZV, CMV, oral prodrug of acyclovir", "500mg", 22.00f, "Rx" },
    { "Famciclovir (Famvir)", "Antiviral", "HSV, VZV shingles, penciclovir prodrug", "250mg", 28.00f, "Rx" },
    { "Ganciclovir (Cytovene)", "Antiviral", "CMV retinitis and prophylaxis", "5mg/kg", 180.00f, "Rx" },
    { "Valganciclovir (Valcyte)", "Antiviral", "CMV prevention and treatment, oral prodrug", "450mg", 280.00f, "Rx" },
    { "Remdesivir (Veklury)", "Antiviral", "COVID-19, nucleotide prodrug RNA polymerase inhibitor", "200mg IV", 2500.00f, "Rx" },
    { "Nirmatrelvir/Ritonavir (Paxlovid)", "Antiviral", "COVID-19, protease inhibitor combination", "300/100mg", 530.00f, "Rx" },
    { "Molnupiravir (Lagevrio)", "Antiviral", "COVID-19, oral RNA polymerase inhibitor", "800mg", 530.00f, "Rx" },
    { "Sofosbuvir (Sovaldi)", "Antiviral", "Hepatitis C, NS5B polymerase inhibitor", "400mg", 850.00f, "Rx" },
    { "Ledipasvir/Sofosbuvir (Harvoni)", "Antiviral", "Hepatitis C genotype 1, NS5A/NS5B inhibitor combo", "90/400mg", 1100.00f, "Rx" },
    { "Bictegravir/FTC/TAF (Biktarvy)", "Antiviral", "HIV-1 infection, single-tablet INI/NRTI regimen", "50/200/25mg", 3800.00f, "Rx" },
    { "Dolutegravir (Tivicay)", "Antiviral", "HIV-1 infection, integrase strand transfer inhibitor", "50mg", 1800.00f, "Rx" },
    { "Darunavir (Prezista)", "Antiviral", "HIV-1, boosted protease inhibitor", "800mg", 1200.00f, "Rx" },
    { "Rilpivirine (Edurant)", "Antiviral", "HIV-1, NNRTI for treatment-naive patients", "25mg", 1500.00f, "Rx" },
    { "Doravirine (Pifeltro)", "Antiviral", "HIV-1 NNRTI for treatment-naive", "100mg", 1600.00f, "Rx" },
    { "Maraviroc (Selzentry)", "Antiviral", "HIV-1 CCR5-tropic, CCR5 antagonist", "300mg", 1400.00f, "Rx" },
    { "Cefdinir (Omnicef)", "Antibiotic", "Community-acquired pneumonia, skin, ear infections", "300mg", 28.00f, "Rx" },
    { "Cefpodoxime (Vantin)", "Antibiotic", "URI, sinusitis, UTI, third-gen cephalosporin", "100mg", 25.00f, "Rx" },
    { "Cefuroxime (Ceftin)", "Antibiotic", "Respiratory, skin, urinary infections", "250mg", 22.00f, "Rx" },
    { "Cefadroxil (Duricef)", "Antibiotic", "Skin, throat, UTI, first-gen cephalosporin", "500mg", 18.00f, "Rx" },
    { "Cefepime (Maxipime)", "Antibiotic", "Hospital-acquired pneumonia, gram-neg, 4th-gen cephalosporin", "2g", 85.00f, "Rx" },
    { "Ceftazidime (Fortaz)", "Antibiotic", "Pseudomonas, gram-neg, 3rd-gen cephalosporin", "1g", 65.00f, "Rx" },
    { "Piperacillin-Tazobactam (Zosyn)", "Antibiotic", "Hospital infections, anti-pseudomonal beta-lactam combo", "3.375g", 95.00f, "Rx" },
    { "Ertapenem (Invanz)", "Antibiotic", "Community infections, carbapenem, ESBL coverage", "1g", 180.00f, "Rx" },
    { "Meropenem (Merrem)", "Antibiotic", "Severe hospital infections, broad-spectrum carbapenem", "1g", 165.00f, "Rx" },
    { "Imipenem-Cilastatin (Primaxin)", "Antibiotic", "Serious hospital infections, carbapenem + renal protector", "500mg", 150.00f, "Rx" },
    { "Linezolid (Zyvox)", "Antibiotic", "VRE, MRSA, gram-positive oxazolidinone", "600mg", 95.00f, "Rx" },
    { "Daptomycin (Cubicin)", "Antibiotic", "MRSA skin infections, bacteremia, lipopeptide", "4mg/kg", 120.00f, "Rx" },
    { "Nitrofurantoin (Macrobid)", "Antibiotic", "Uncomplicated UTI, oral nitrofuran", "100mg", 15.00f, "Rx" },
    { "Fosfomycin (Monurol)", "Antibiotic", "Uncomplicated UTI, single-dose, phosphonic acid", "3g", 45.00f, "Rx" },
    { "Rifaximin (Xifaxan)", "Antibiotic", "Traveler's diarrhea, HE prophylaxis, IBS-D", "550mg", 180.00f, "Rx" },
    { "Mupirocin (Bactroban)", "Antibiotic", "Impetigo, nasal MRSA decolonization, topical", "2%", 18.00f, "Rx" },
    { "Fidaxomicin (Dificid)", "Antibiotic", "C. difficile infection, macrolide antibiotic", "200mg", 3200.00f, "Rx" },
    { "Trimethoprim (Primsol)", "Antibiotic", "UTI, Pneumocystis prophylaxis", "100mg", 12.00f, "Rx" },
    { "Itraconazole (Sporanox)", "Antifungal", "Aspergillus, blastomycosis, histoplasma, triazole", "100mg", 45.00f, "Rx" },
    { "Posaconazole (Noxafil)", "Antifungal", "Aspergillus prophylaxis, invasive fungal infections", "300mg", 280.00f, "Rx" },
    { "Isavuconazole (Cresemba)", "Antifungal", "Aspergillosis, mucormycosis, broad triazole", "372mg", 380.00f, "Rx" },
    { "Anidulafungin (Eraxis)", "Antifungal", "Candida infections, echinocandin class", "200mg load", 250.00f, "Rx" },
    { "Micafungin (Mycamine)", "Antifungal", "Candidemia, Aspergillus prophylaxis, echinocandin", "100mg", 240.00f, "Rx" },
    { "Caspofungin (Cancidas)", "Antifungal", "Invasive Aspergillus, Candida, echinocandin", "70mg load", 230.00f, "Rx" },
    { "Terbinafine (Lamisil)", "Antifungal", "Onychomycosis, tinea, allylamine antifungal", "250mg", 22.00f, "Rx" },
    { "Griseofulvin (Grifulvin)", "Antifungal", "Tinea capitis, dermatophyte infections, oral", "500mg", 18.00f, "Rx" },
    { "Ciclopirox (Penlac)", "Antifungal", "Onychomycosis, nail lacquer antifungal", "8%", 28.00f, "Rx" },
    { "Butenafine (Lotrimin Ultra)", "Antifungal", "Athlete's foot, tinea, benzylamine antifungal", "1%", 9.99f, "OTC" },
    { "Atovaquone/Proguanil (Malarone)", "Antibiotic", "Malaria prophylaxis and treatment", "250/100mg", 95.00f, "Rx" },
    { "Artemether-Lumefantrine (Coartem)", "Antibiotic", "Uncomplicated malaria treatment", "20/120mg", 45.00f, "Rx" },
    { "Tafenoquine (Krintafel)", "Antibiotic", "P. vivax malaria radical cure, 8-aminoquinoline", "300mg", 120.00f, "Rx" },
    { "Albendazole (Albenza)", "Antibiotic", "Helminth infections, neurocysticercosis", "400mg", 18.00f, "Rx" },
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
    { "Pantoprazole (Protonix)", "Digestive", "GERD, peptic ulcer, Zollinger-Ellison, PPI", "40mg", 15.00f, "Rx" },
    { "Rabeprazole (Aciphex)", "Digestive", "GERD, peptic ulcer, H. pylori, PPI", "20mg", 18.00f, "Rx" },
    { "Dexlansoprazole (Dexilant)", "Digestive", "GERD, erosive esophagitis, dual-release PPI", "30mg", 22.00f, "Rx" },
    { "Misoprostol (Cytotec)", "Digestive", "Peptic ulcer prevention with NSAIDs, GI protection", "200mcg", 18.00f, "Rx" },
    { "Sucralfate (Carafate)", "Digestive", "Peptic ulcer, forms protective coating", "1g", 12.00f, "Rx" },
    { "Prucalopride (Motegrity)", "Digestive", "Chronic idiopathic constipation, 5-HT4 agonist", "2mg", 95.00f, "Rx" },
    { "Linaclotide (Linzess)", "Digestive", "IBS-C, chronic idiopathic constipation, GC-C agonist", "145mcg", 380.00f, "Rx" },
    { "Lubiprostone (Amitiza)", "Digestive", "Chronic constipation, IBS-C, chloride channel activator", "8mcg", 280.00f, "Rx" },
    { "Methylnaltrexone (Relistor)", "Digestive", "Opioid-induced constipation, peripheral mu antagonist", "8mg", 380.00f, "Rx" },
    { "Mesalamine (Asacol)", "Digestive", "Ulcerative colitis maintenance, 5-aminosalicylate", "400mg", 120.00f, "Rx" },
    { "Budesonide (Entocort)", "Digestive", "Crohn's disease, microscopic colitis, oral steroid", "3mg", 95.00f, "Rx" },
    { "Ursodiol (UDCA)", "Digestive", "Primary biliary cholangitis, gallstones, bile acid", "300mg", 45.00f, "Rx" },
    { "Cholestyramine (Questran)", "Blood", "High LDL, bile acid sequestrant, diarrhea", "4g", 22.00f, "Rx" },
    { "Colesevelam (Welchol)", "Blood", "High LDL, type 2 diabetes, bile acid sequestrant", "625mg", 65.00f, "Rx" },
    { "Ondansetron (Zofran)", "Digestive", "Chemotherapy-induced nausea, PONV, 5-HT3 antagonist", "4mg", 18.00f, "Rx" },
    { "Granisetron (Kytril)", "Digestive", "Chemotherapy/radiation nausea, 5-HT3 antagonist", "1mg", 28.00f, "Rx" },
    { "Palonosetron (Aloxi)", "Digestive", "Chemotherapy nausea, long-acting 5-HT3 antagonist", "0.25mg IV", 180.00f, "Rx" },
    { "Aprepitant (Emend)", "Digestive", "Chemotherapy nausea, NK1 receptor antagonist", "125mg", 95.00f, "Rx" },
    { "Pancrelipase (Creon)", "Digestive", "Exocrine pancreatic insufficiency, enzyme replacement", "6000u", 280.00f, "Rx" },
    { "Polyethylene Glycol (MiraLax)", "Digestive", "Constipation, osmotic laxative", "17g", 12.99f, "OTC" },
    { "Senna (Senokot)", "Digestive", "Constipation, stimulant laxative", "8.6mg", 6.99f, "OTC" },
    { "Bisacodyl (Dulcolax)", "Digestive", "Constipation, bowel prep, stimulant laxative", "5mg", 7.49f, "OTC" },
    { "Docusate (Colace)", "Digestive", "Constipation, stool softener", "100mg", 6.49f, "OTC" },
    { "Latanoprost (Xalatan)", "Eye/ENT", "Open-angle glaucoma, ocular hypertension, prostaglandin", "0.005%", 22.00f, "Rx" },
    { "Bimatoprost (Lumigan)", "Eye/ENT", "Glaucoma, eyelash growth, prostaglandin analog", "0.01%", 28.00f, "Rx" },
    { "Travoprost (Travatan)", "Eye/ENT", "Glaucoma, IOP lowering, prostaglandin FP agonist", "0.004%", 25.00f, "Rx" },
    { "Timolol (Timoptic)", "Eye/ENT", "Glaucoma, IOP reduction, beta-blocker drops", "0.5%", 12.00f, "Rx" },
    { "Dorzolamide (Trusopt)", "Eye/ENT", "Glaucoma, carbonic anhydrase inhibitor drops", "2%", 18.00f, "Rx" },
    { "Brimonidine (Alphagan)", "Eye/ENT", "Glaucoma, IOP lowering, alpha-2 agonist drops", "0.2%", 22.00f, "Rx" },
    { "Netarsudil (Rhopressa)", "Eye/ENT", "Glaucoma, ROCK/NET inhibitor drops", "0.02%", 95.00f, "Rx" },
    { "Ranibizumab (Lucentis)", "Eye/ENT", "AMD, DME, anti-VEGF intravitreal injection", "0.5mg", 1900.00f, "Rx" },
    { "Aflibercept (Eylea)", "Eye/ENT", "AMD, DME, VEGF trap intravitreal injection", "2mg", 1850.00f, "Rx" },
    { "Faricimab (Vabysmo)", "Eye/ENT", "AMD and DME, bispecific anti-VEGF/Ang-2", "6mg", 2200.00f, "Rx" },
    { "Olopatadine (Pataday)", "Eye/ENT", "Allergic conjunctivitis, H1 antihistamine drops", "0.1%", 12.00f, "Rx" },
    { "Moxifloxacin (Vigamox)", "Eye/ENT", "Bacterial conjunctivitis, fluoroquinolone drops", "0.5%", 18.00f, "Rx" },
    { "Fluorometholone (FML)", "Eye/ENT", "Ocular inflammation, corticosteroid drops", "0.1%", 15.00f, "Rx" },
    { "Prednisolone Acetate (Pred Forte)", "Eye/ENT", "Ocular inflammation post-surgery", "1%", 18.00f, "Rx" },
    { "Fluticasone Nasal (Flonase)", "Eye/ENT", "Allergic rhinitis, nasal corticosteroid spray", "50mcg/spray", 12.99f, "OTC" },
    { "Mometasone Nasal (Nasonex)", "Eye/ENT", "Allergic rhinitis, nasal polyps, corticosteroid", "50mcg/spray", 15.00f, "Rx" },
    { "Azelastine Nasal (Astelin)", "Eye/ENT", "Allergic rhinitis, antihistamine nasal spray", "137mcg", 22.00f, "Rx" },
    { "Oxymetazoline (Afrin)", "Eye/ENT", "Nasal congestion, decongestant nasal spray", "0.05%", 7.99f, "OTC" },
    { "Tretinoin (Retin-A)", "Skin", "Acne, photoaging, vitamin A derivative topical", "0.025%", 25.00f, "Rx" },
    { "Adapalene (Differin)", "Skin", "Acne, retinoid, reduces comedones and inflammation", "0.1%", 14.99f, "OTC" },
    { "Tazarotene (Tazorac)", "Skin", "Psoriasis, acne, wrinkles, topical retinoid", "0.1%", 45.00f, "Rx" },
    { "Isotretinoin (Accutane)", "Skin", "Severe acne, oral retinoid, teratogenic", "20mg", 85.00f, "Rx" },
    { "Azelaic Acid (Finacea)", "Skin", "Acne, rosacea, antimicrobial and anti-inflammatory", "15%", 18.00f, "Rx" },
    { "Calcipotriol (Dovonex)", "Skin", "Psoriasis, vitamin D analog topical", "0.005%", 38.00f, "Rx" },
    { "Clascoterone (Winlevi)", "Skin", "Acne, androgen receptor inhibitor topical", "1%", 85.00f, "Rx" },
    { "Sarecycline (Seysara)", "Skin", "Acne, narrow-spectrum tetracycline antibiotic", "60mg", 95.00f, "Rx" },
    { "Deucravacitinib (Sotyktu)", "Skin", "Plaque psoriasis, TYK2 inhibitor oral", "6mg", 1800.00f, "Rx" },
    { "Spesolimab (Spevigo)", "Skin", "Generalized pustular psoriasis, IL-36R inhibitor", "900mg IV", 18000.00f, "Rx" },
    { "Tapinarof (Vtama)", "Skin", "Plaque psoriasis, AhR agonist topical", "1%", 280.00f, "Rx" },
    { "Tralokinumab (Adbry)", "Skin", "Atopic dermatitis, IL-13 antagonist", "300mg q2wk", 2800.00f, "Rx" },
    { "Lebrikizumab (Ebglyss)", "Skin", "Moderate-severe atopic dermatitis, IL-13 monoclonal", "250mg q2wk", 3000.00f, "Rx" },
    { "Octreotide (Sandostatin)", "Hormonal", "Acromegaly, carcinoid tumors, GI bleeding, somatostatin analog", "50mcg", 120.00f, "Rx" },
    { "Cabergoline (Dostinex)", "Hormonal", "Hyperprolactinemia, Parkinson's, dopamine agonist", "0.25mg", 45.00f, "Rx" },
    { "Bromocriptine (Cycloset)", "Hormonal", "Hyperprolactinemia, Parkinson's, type 2 diabetes", "2.5mg", 28.00f, "Rx" },
    { "Cinacalcet (Sensipar)", "Hormonal", "Secondary hyperparathyroidism, calcimimetic", "30mg", 180.00f, "Rx" },
    { "Vitamin D3 (Cholecalciferol)", "Vitamin", "Vitamin D deficiency, bone health, immune support", "1000IU", 5.99f, "OTC" },
    { "Vitamin B12 (Cyanocobalamin)", "Vitamin", "B12 deficiency, pernicious anemia, neuropathy", "1000mcg", 8.99f, "OTC" },
    { "Vitamin C (Ascorbic Acid)", "Vitamin", "Scurvy, antioxidant, immune support, wound healing", "500mg", 6.99f, "OTC" },
    { "Zinc Gluconate", "Vitamin", "Immune support, wound healing, zinc deficiency", "50mg", 7.49f, "OTC" },
    { "Magnesium Glycinate", "Vitamin", "Magnesium deficiency, muscle cramps, sleep, migraines", "400mg", 14.99f, "OTC" },
    { "Omega-3 FA (Lovaza)", "Blood", "Hypertriglyceridemia, cardiovascular support", "4g/day", 85.00f, "Rx" },
    { "N-Acetylcysteine (NAC)", "Respiratory", "Acetaminophen overdose, COPD mucus, antioxidant", "600mg", 12.99f, "OTC" },
    { "Iron Polysaccharide (Niferex)", "Blood", "Iron deficiency anemia, well-tolerated oral iron", "150mg", 12.99f, "OTC" },
    { "Ferric Carboxymaltose (Injectafer)", "Blood", "Iron deficiency anemia, IV iron for repletion", "750mg IV", 380.00f, "Rx" },
    { "Milnacipran (Savella)", "Pain Relief", "Fibromyalgia, SNRI for pain modulation", "50mg", 95.00f, "Rx" },
    { "Ziconotide (Prialt)", "Pain Relief", "Intractable chronic pain, intrathecal calcium blocker", "100mcg/day IT", 8500.00f, "Rx" },
    { "Capsaicin Patch (Qutenza)", "Pain Relief", "Neuropathic pain, peripheral TRPV1 desensitizer", "8%", 650.00f, "Rx" },
    { "Dextromethorphan/Quinidine (Nuedexta)", "Neurological", "Pseudobulbar affect, involuntary laughing/crying", "20/10mg", 280.00f, "Rx" },
    { "Naloxegol (Movantik)", "Digestive", "Opioid-induced constipation, PAMORA", "12.5mg", 180.00f, "Rx" },
    { "Vedolizumab (Entyvio)", "Immune", "UC, Crohn's, gut-selective integrin antagonist", "300mg q8wk", 3800.00f, "Rx" },
    { "Ozanimod (Zeposia)", "Immune", "UC, MS, S1P receptor modulator", "0.92mg", 2800.00f, "Rx" },
    { "Ezetimibe (Zetia)", "Blood", "High LDL, cholesterol absorption inhibitor", "10mg", 45.00f, "Rx" },
    { "Colchicine (Lodoco)", "Heart", "Atherosclerosis prevention, anti-inflammatory", "0.5mg", 48.00f, "Rx" },
    { "Tacrolimus (Prograf)", "Immune", "Organ transplant rejection prevention, calcineurin inhibitor", "1mg", 45.00f, "Rx" },
    { "Cyclosporine (Sandimmune)", "Immune", "Transplant, autoimmune diseases, calcineurin inhibitor", "25mg", 38.00f, "Rx" },
    { "Mycophenolate (CellCept)", "Immune", "Transplant rejection, lupus nephritis, inosine synthesis", "500mg", 35.00f, "Rx" },
    { "Sirolimus (Rapamune)", "Immune", "Renal transplant, mTOR inhibitor immunosuppressant", "1mg", 85.00f, "Rx" },
    { "Azathioprine (Imuran)", "Immune", "Transplant, RA, IBD, purine synthesis inhibitor", "50mg", 22.00f, "Rx" },
    { "Leflunomide (Arava)", "Immune", "Rheumatoid arthritis, pyrimidine synthesis inhibitor", "10mg", 28.00f, "Rx" },
    { "Hydroxychloroquine (Plaquenil)", "Immune", "RA, lupus, malaria, antimalarial/immunomodulator", "200mg", 18.00f, "Rx" },
    { "Sulfasalazine (Azulfidine)", "Immune", "RA, UC, Crohn's, aminosalicylate + sulfonamide", "500mg", 22.00f, "Rx" },
    { "Canakinumab (Ilaris)", "Immune", "CAPS, gout flares, IL-1 beta monoclonal antibody", "150mg q8wk", 18000.00f, "Rx" },
    { "Anakinra (Kineret)", "Immune", "RA, CAPS, neonatal onset MAS, IL-1 receptor antagonist", "100mg/day", 850.00f, "Rx" },
    { "Budesonide/Formoterol (Symbicort)", "Respiratory", "Asthma, COPD, ICS/LABA combination inhaler", "160/4.5mcg", 280.00f, "Rx" },
    { "Fluticasone/Salmeterol (Advair)", "Respiratory", "Asthma, COPD, ICS/LABA diskus inhaler", "250/50mcg", 260.00f, "Rx" },
    { "Fluticasone/Vilanterol (Breo)", "Respiratory", "Asthma, COPD, once-daily ICS/LABA inhaler", "100/25mcg", 280.00f, "Rx" },
    { "Umeclidinium (Incruse)", "Respiratory", "COPD, once-daily LAMA bronchodilator", "62.5mcg", 180.00f, "Rx" },
    { "Aclidinium (Tudorza)", "Respiratory", "COPD, twice-daily LAMA bronchodilator", "400mcg", 160.00f, "Rx" },
    { "Indacaterol (Arcapta)", "Respiratory", "COPD, once-daily ultra-LABA bronchodilator", "75mcg", 180.00f, "Rx" },
    { "Beclomethasone (QVAR)", "Respiratory", "Asthma, inhaled corticosteroid, HFA formulation", "80mcg", 120.00f, "Rx" },
    { "Ciclesonide (Alvesco)", "Respiratory", "Asthma, pro-drug ICS minimal systemic effects", "80mcg", 140.00f, "Rx" },
    { "Dexamethasone (Decadron)", "Immune", "Inflammation, cerebral edema, immunosuppression", "4mg", 8.00f, "Rx" },
    { "Fludrocortisone (Florinef)", "Hormonal", "Adrenal insufficiency, mineralocorticoid replacement", "0.1mg", 18.00f, "Rx" },
    { "Hydrocortisone (Solu-Cortef)", "Immune", "Adrenal crisis, severe inflammation, IV steroid", "100mg IV", 12.00f, "Rx" },
    { "Betamethasone (Celestone)", "Immune", "Fetal lung maturity, anti-inflammatory, joint injection", "6mg", 15.00f, "Rx" },
    { "Fluocinolone (Synalar)", "Skin", "Eczema, psoriasis, mid-potency topical steroid", "0.025%", 18.00f, "Rx" },
    { "Clobetasol (Temovate)", "Skin", "Severe psoriasis, eczema, ultra-high-potency topical steroid", "0.05%", 22.00f, "Rx" },
    { "Triamcinolone (Kenalog)", "Immune", "Allergies, joint inflammation, corticosteroid injection", "40mg/mL", 15.00f, "Rx" },
    { "Methylprednisolone (Medrol)", "Immune", "Acute inflammation, asthma, spinal injury, steroid", "4mg", 12.00f, "Rx" },
    { "Digoxin (Lanoxin)", "Heart", "Atrial fibrillation rate control, heart failure", "0.125mg", 14.00f, "Rx" },
    { "Phentermine/Topiramate (Qsymia)", "Hormonal", "Obesity, weight management combination", "3.75/23mg", 95.00f, "Rx" },
    { "Naltrexone/Bupropion (Contrave)", "Hormonal", "Obesity, weight management, CNS combination", "8/90mg", 85.00f, "Rx" },
    { "Orlistat (Xenical)", "Digestive", "Obesity, lipase inhibitor, reduces fat absorption", "120mg", 45.00f, "Rx" },
    { "Benzonatate (Tessalon)", "Respiratory", "Cough suppression, non-narcotic anesthetic", "100mg", 12.00f, "Rx" },
    { "Guaifenesin (Mucinex)", "Respiratory", "Chest congestion, expectorant, thins mucus", "600mg", 9.99f, "OTC" },
    { "Dextromethorphan (Robitussin DM)", "Respiratory", "Cough suppression, NMDA antagonist antitussive", "15mg", 7.99f, "OTC" },
    { "Brompheniramine (Dimetapp)", "Allergy", "Allergies, hay fever, first-gen antihistamine", "4mg", 7.99f, "OTC" },
    { "Carbinoxamine (Palgic)", "Allergy", "Allergic rhinitis, urticaria, antihistamine", "4mg", 12.00f, "Rx" },
    { "Promethazine (Phenergan)", "Allergy", "Nausea, motion sickness, allergies, phenothiazine", "25mg", 12.00f, "Rx" },
    { "Hydroxyzine (Atarax)", "Allergy", "Anxiety, urticaria, sedation, antihistamine", "25mg", 12.00f, "Rx" },
    { "Terfenadine (historical)", "Allergy", "Allergic rhinitis, withdrawn due to cardiac risk", "60mg", 0.00f, "Rx" },
    { "Levocetirizine (Xyzal)", "Allergy", "Allergic rhinitis, urticaria, active cetirizine isomer", "5mg", 12.99f, "OTC" },
    { "Benzocaine (Orajel)", "Pain Relief", "Topical anesthesia, teething, sore throat", "20%", 7.99f, "OTC" },
    { "Lidocaine Patch (Lidoderm)", "Pain Relief", "Postherpetic neuralgia, local anesthesia topical patch", "5%", 85.00f, "Rx" },
    { "Bupivacaine (Marcaine)", "Pain Relief", "Regional anesthesia, nerve blocks, local anesthetic", "0.25%", 12.00f, "Rx" },
    { "Ropivacaine (Naropin)", "Pain Relief", "Regional anesthesia, epidural, local anesthetic", "0.2%", 18.00f, "Rx" },
    { "Succinylcholine (Anectine)", "Emergency", "Rapid sequence intubation, depolarizing neuromuscular blocker", "1mg/kg", 22.00f, "Rx" },
    { "Rocuronium (Zemuron)", "Emergency", "Neuromuscular blockade, intubation, non-depolarizing", "0.6mg/kg", 28.00f, "Rx" },
    { "Propofol (Diprivan)", "Emergency", "IV anesthesia induction, ICU sedation", "10mg/mL", 35.00f, "Rx" },
    { "Dexmedetomidine (Precedex)", "Emergency", "ICU sedation, alpha-2 agonist, cooperative sedation", "4mcg/mL", 95.00f, "Rx" },
    { "Midazolam (Versed)", "Emergency", "Procedural sedation, pre-anesthetic, IV benzodiazepine", "1mg", 8.00f, "Rx" },
    { "Phenobarbital (Luminal)", "Neurological", "Epilepsy, status epilepticus, barbiturate", "30mg", 12.00f, "Rx" },
    { "Zonisamide (Zonegran)", "Neurological", "Epilepsy, partial seizures, sulfonamide anticonvulsant", "25mg", 35.00f, "Rx" },
    { "Vigabatrin (Sabril)", "Neurological", "Infantile spasms, complex partial seizures, GABA analog", "500mg", 120.00f, "Rx" },
    { "Lacosamide (Vimpat)", "Neurological", "Partial seizures, sodium channel slow inactivator", "100mg", 85.00f, "Rx" },
    { "Perampanel (Fycompa)", "Neurological", "Epilepsy, AMPA receptor antagonist", "2mg", 95.00f, "Rx" },
    { "Brivaracetam (Briviact)", "Neurological", "Epilepsy, SV2A modulator similar to levetiracetam", "50mg", 110.00f, "Rx" },
    { "Cenobamate (Xcopri)", "Neurological", "Focal seizures, dual mechanism anticonvulsant", "12.5mg", 750.00f, "Rx" },
    { "Propranolol (Inderal)", "Heart", "Hypertension, angina, tremor, migraine prophylaxis", "10mg", 8.00f, "Rx" },
    { "Nadolol (Corgard)", "Heart", "Hypertension, angina, non-selective beta-blocker", "20mg", 12.00f, "Rx" },
    { "Bisoprolol (Zebeta)", "Heart", "Hypertension, heart failure, cardioselective beta-blocker", "5mg", 15.00f, "Rx" },
    { "Nebivolol (Bystolic)", "Heart", "Hypertension, vasodilating beta-1 selective blocker", "5mg", 35.00f, "Rx" },
    { "Carvedilol (Coreg)", "Heart", "Heart failure, hypertension, alpha/beta blocker", "3.125mg", 12.00f, "Rx" },
    { "Diltiazem (Cardizem)", "Heart", "Hypertension, angina, rate control, non-DHP CCB", "120mg", 18.00f, "Rx" },
    { "Verapamil (Calan)", "Heart", "SVT, hypertension, angina, migraine, CCB", "80mg", 14.00f, "Rx" },
    { "Felodipine (Plendil)", "Heart", "Hypertension, DHP calcium channel blocker", "5mg", 15.00f, "Rx" },
    { "Nifedipine (Procardia)", "Heart", "Hypertension, angina, DHP CCB, Raynaud's", "10mg", 12.00f, "Rx" },
    { "Nicardipine (Cardene)", "Heart", "Hypertension emergency, CCB IV infusion", "20mg", 35.00f, "Rx" },
    { "Olmesartan (Benicar)", "Heart", "Hypertension, ARB, angiotensin II receptor blocker", "20mg", 35.00f, "Rx" },
    { "Azilsartan (Edarbi)", "Heart", "Hypertension, ARB, highest BP-lowering ARB", "40mg", 45.00f, "Rx" },
    { "Sacubitril (Entresto component)", "Heart", "Combined with valsartan for heart failure", "24mg", 480.00f, "Rx" },
    { "Hydralazine (Apresoline)", "Heart", "Hypertension, heart failure adjunct, vasodilator", "10mg", 8.00f, "Rx" },
    { "Minoxidil (Rogaine)", "Heart", "Severe hypertension, hair loss, vasodilator", "5mg", 18.00f, "OTC" },
    { "Alpha-Methyldopa (Aldomet)", "Heart", "Hypertension in pregnancy, central alpha-2 agonist", "250mg", 12.00f, "Rx" },
    { "Clonidine (Catapres)", "Heart", "Hypertension, ADHD, alpha-2 agonist, opioid withdrawal", "0.1mg", 8.00f, "Rx" },
    { "Doxazosin (Cardura)", "Heart", "Hypertension, BPH, alpha-1 blocker", "1mg", 12.00f, "Rx" },
    { "Terazosin (Hytrin)", "Heart", "Hypertension, BPH, alpha-1 selective blocker", "1mg", 10.00f, "Rx" },
    { "Prazosin (Minipress)", "Heart", "Hypertension, PTSD nightmares, alpha-1 blocker", "1mg", 8.00f, "Rx" },
    { "Phenoxybenzamine (Dibenzyline)", "Heart", "Pheochromocytoma, irreversible alpha-blocker", "10mg", 1800.00f, "Rx" },
    { "Phentolamine (Oraverse)", "Heart", "Pheochromocytoma crisis, local anesthesia reversal", "5mg", 35.00f, "Rx" },
    { "Allopurinol (Zyloprim)", "Pain Relief", "Gout prevention, xanthine oxidase inhibitor", "100mg", 8.00f, "Rx" },
    { "Rasburicase (Elitek)", "Oncology", "Tumor lysis syndrome prevention, urate oxidase", "0.2mg/kg", 2800.00f, "Rx" },
    { "Lesinurad (Zurampic)", "Pain Relief", "Gout, URAT1 inhibitor, uricosuric", "200mg", 250.00f, "Rx" },
    { "Drospirenone/Ethinyl Estradiol (Yasmin)", "Women's Health", "Combined oral contraceptive, anti-androgenic", "3/0.03mg", 38.00f, "Rx" },
    { "Medroxyprogesterone (Depo-Provera)", "Women's Health", "Contraception, endometriosis, injectable progestin", "150mg/3mo", 45.00f, "Rx" },
    { "Ulipristal Acetate (ella)", "Women's Health", "Emergency contraception within 5 days", "30mg", 52.00f, "Rx" },
    { "Ospemifene (Osphena)", "Women's Health", "Dyspareunia, vaginal atrophy, SERM", "60mg", 95.00f, "Rx" },
    { "Prasterone (Intrarosa)", "Women's Health", "Vaginal atrophy, intravaginal DHEA", "6.5mg", 120.00f, "Rx" },
    { "Fezolinetant (Veozah)", "Women's Health", "Menopausal hot flashes, neurokinin B receptor antagonist", "45mg", 180.00f, "Rx" },
    { "Avanafil (Stendra)", "Men's Health", "Erectile dysfunction, PDE5 inhibitor fast-onset", "50mg", 72.00f, "Rx" },
    { "Alprostadil (Muse)", "Men's Health", "Erectile dysfunction, urethral or injection prostaglandin", "500mcg", 95.00f, "Rx" },
    { "Degarelix (Firmagon)", "Men's Health", "Prostate cancer, GnRH antagonist, ADT", "240mg", 850.00f, "Rx" },
    { "Leuprolide (Lupron)", "Men's Health", "Prostate cancer, endometriosis, uterine fibroids, GnRH agonist", "7.5mg", 1200.00f, "Rx" },
    { "Bicalutamide (Casodex)", "Men's Health", "Prostate cancer, androgen receptor antagonist", "50mg", 35.00f, "Rx" },
    { "Potassium Chloride (Klor-Con)", "Vitamin", "Hypokalemia, potassium replacement", "20mEq", 8.99f, "Rx" },
    { "Magnesium Oxide", "Vitamin", "Hypomagnesemia, constipation, migraine prevention", "400mg", 6.99f, "OTC" },
    { "Phosphorus Supplement", "Vitamin", "Hypophosphatemia, bone and energy metabolism", "250mg", 9.99f, "OTC" },
    { "Thiamine (Vitamin B1)", "Vitamin", "Wernicke encephalopathy, beriberi, B1 deficiency", "100mg", 5.99f, "OTC" },
    { "Riboflavin (Vitamin B2)", "Vitamin", "B2 deficiency, migraine prophylaxis, antioxidant", "400mg", 8.99f, "OTC" },
    { "Niacin (Vitamin B3)", "Vitamin", "High triglycerides, pellagra, LDL reduction", "500mg", 10.99f, "OTC" },
    { "Pyridoxine (Vitamin B6)", "Vitamin", "B6 deficiency, morning sickness, isoniazid neuropathy", "50mg", 6.99f, "OTC" },
    { "Biotin (Vitamin B7)", "Vitamin", "Biotin deficiency, hair and nail health", "5000mcg", 9.99f, "OTC" },
    { "Iron Sucrose (Venofer)", "Blood", "Iron deficiency anemia in CKD, IV iron complex", "200mg IV", 280.00f, "Rx" },
    { "Phytonadione (Vitamin K1)", "Blood", "Coagulation factor deficiency, warfarin reversal", "10mg", 12.00f, "Rx" },
    { "Protamine Sulfate", "Emergency", "Heparin reversal, fish-derived protein antagonist", "50mg IV", 35.00f, "Rx" },
    { "Idarucizumab (Praxbind)", "Emergency", "Dabigatran reversal, specific antibody fragment", "5g IV", 3200.00f, "Rx" },
    { "Andexanet Alfa (Andexxa)", "Emergency", "Factor Xa inhibitor reversal (apixaban, rivaroxaban)", "400mg IV", 28000.00f, "Rx" },
    { "Sugammadex (Bridion)", "Emergency", "Rocuronium/vecuronium reversal, cyclodextrin", "200mg IV", 85.00f, "Rx" },
    { "Vasopressin (DDAVP nasal)", "Emergency", "Central DI, enuresis, intranasal formulation", "10mcg", 28.00f, "Rx" },
    { "Norepinephrine (Levophed)", "Emergency", "Septic shock vasopressor, alpha-1/beta-1 agonist", "4mg IV", 25.00f, "Rx" },
    { "Dopamine", "Emergency", "Cardiogenic shock, renal perfusion, catecholamine", "200mg IV", 18.00f, "Rx" },
    { "Dobutamine (Dobutrex)", "Emergency", "Acute decompensated heart failure, beta-1 inotrope", "250mg IV", 22.00f, "Rx" },
    { "Phenylephrine (Neo-Synephrine)", "Emergency", "Septic shock, nasal congestion, alpha-1 agonist", "10mg IV", 18.00f, "Rx" },
    { "Milrinone (Primacor)", "Emergency", "Acute heart failure, phosphodiesterase 3 inhibitor", "10mg IV", 28.00f, "Rx" },
    { "Levosimendan (Simdax)", "Emergency", "Acute heart failure, calcium sensitizer", "12.5mg", 380.00f, "Rx" },
    { "Protamine (Reversal)", "Emergency", "Heparin reversal agent in cardiac surgery", "50mg IV", 30.00f, "Rx" },
    { "Tranexamic Acid (TXA)", "Blood", "Surgical bleeding, trauma hemorrhage, antifibrinolytic", "1g IV", 18.00f, "Rx" },
    { "Aminocaproic Acid (Amicar)", "Blood", "Excessive bleeding, antifibrinolytic agent", "1g IV", 22.00f, "Rx" },
    { "Factor VIIa (NovoSeven)", "Blood", "Hemophilia, uncontrolled bleeding, recombinant", "1mg", 3800.00f, "Rx" },
    { "Romiplostim (Nplate)", "Blood", "Immune thrombocytopenia, TPO receptor agonist", "250mcg/wk", 2800.00f, "Rx" },
    { "Eltrombopag (Promacta)", "Blood", "ITP, aplastic anemia, oral TPO receptor agonist", "25mg", 2200.00f, "Rx" },
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
    { "Buspirone (Buspar)", "Mental Health", "Generalized anxiety disorder, 5-HT1A partial agonist", "5mg", 12.00f, "Rx" },
    { "Gabapentin (Neurontin)", "Neurological", "Epilepsy, neuropathic pain, alpha-2-delta ligand", "100mg", 12.00f, "Rx" },
    { "Pregabalin (Lyrica)", "Neurological", "Neuropathic pain, fibromyalgia, seizures, anxiolytic", "75mg", 35.00f, "Rx" },
    { "Baclofen (Lioresal)", "Muscle Relaxant", "Spasticity, muscle spasms, GABA-B agonist", "10mg", 12.00f, "Rx" },
    { "Cyclobenzaprine (Flexeril)", "Muscle Relaxant", "Acute muscle spasm, tricyclic-related muscle relaxant", "5mg", 10.00f, "Rx" },
    { "Tizanidine (Zanaflex)", "Muscle Relaxant", "Spasticity, alpha-2 agonist muscle relaxant", "2mg", 12.00f, "Rx" },
    { "Methocarbamol (Robaxin)", "Muscle Relaxant", "Muscle spasm, carbamate muscle relaxant", "500mg", 12.00f, "Rx" },
    { "Orphenadrine (Norflex)", "Muscle Relaxant", "Muscle spasm, antihistamine mechanism", "100mg", 14.00f, "Rx" },
    { "Carisoprodol (Soma)", "Muscle Relaxant", "Acute musculoskeletal pain, GABA-A modulator", "350mg", 15.00f, "Rx" },
    { "Diazepam (Valium IV)", "Emergency", "Status epilepticus, alcohol withdrawal IV benzodiazepine", "10mg IV", 8.00f, "Rx" },
    { "Lorazepam (Ativan IV)", "Emergency", "Status epilepticus, acute agitation, IV benzodiazepine", "2mg IV", 8.00f, "Rx" },
    { "Pantoprazole IV (Protonix IV)", "Digestive", "Active GI bleeding, stress ulcer prophylaxis IV PPI", "40mg IV", 18.00f, "Rx" },
    { "Famotidine IV (Pepcid IV)", "Digestive", "GI bleeding, stress ulcer prophylaxis IV H2 blocker", "20mg IV", 12.00f, "Rx" },
    { "Cyproheptadine (Periactin)", "Allergy", "Allergies, appetite stimulant, serotonin syndrome adjunct", "4mg", 10.00f, "Rx" },
    { "Ipratropium (Atrovent)", "Respiratory", "COPD, asthma, bronchodilator, muscarinic antagonist", "0.02%", 38.00f, "Rx" },
    { "Tiotropium (Spiriva)", "Respiratory", "COPD maintenance, once-daily LAMA", "18mcg", 180.00f, "Rx" },
    { "Glycopyrrolate (Robinul)", "Respiratory", "COPD, LAMA bronchodilator, anticholinergic", "0.2mg", 15.00f, "Rx" },
    { "Revefenacin (Yupelri)", "Respiratory", "COPD, once-daily nebulized LAMA", "175mcg", 180.00f, "Rx" },
    { "Salmeterol (Serevent)", "Respiratory", "Asthma maintenance, COPD, long-acting beta-2 agonist", "50mcg", 85.00f, "Rx" },
    { "Formoterol (Foradil)", "Respiratory", "Asthma, COPD, rapid-onset LABA bronchodilator", "12mcg", 75.00f, "Rx" },
    { "Olodaterol (Striverdi)", "Respiratory", "COPD maintenance, once-daily LABA", "2.5mcg", 180.00f, "Rx" },
    { "Albuterol/Ipratropium (Combivent)", "Respiratory", "COPD, combined SABA/SAMA bronchodilator", "103/18mcg", 95.00f, "Rx" },
"""

# Combine all medicines (no duplicates by name)
all_lines = []
seen = set()
for line in (old_meds + '\n' + extra_meds + '\n' + extra_meds2).split('\n'):
    s = line.strip()
    if not s or not s.startswith('{'):
        continue
    # Extract name as dedup key
    try:
        name = s.split('"')[1].lower()
    except Exception:
        name = s
    if name in seen:
        continue
    seen.add(name)
    if not s.endswith(','):
        s += ','
    all_lines.append('    ' + s)

print(f'{len(all_lines)} unique medicines')

meds_block = '\n'.join(all_lines)
full = cpp_header + meds_block + cpp_footer

out = r'C:/Users/Karson/Desktop/Orbit Projects/Orbit-Medicin/main.cpp'
with open(out, 'w', encoding='utf-8') as f:
    f.write(full)
print(f'Wrote {len(full.splitlines())} lines')
