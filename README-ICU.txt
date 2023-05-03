The ICU module here is a copy of ext/icu/icu.c with some changes.

It can be compiled to a standalone icuwin.dll loadable extension.
Cannot be called icu on Windows if linking to the Windows ICU


The following functionality has been added:

icu_enumLocale()    -- Number of Locale's recognized by Collator
icu_enumLocale(n)   -- Locale n name 0 <= n < icu_enumLocals()

icu_trans(<transform>,<string>)   returns transliterated string
    <transform> is the transform to apply

fold(<string>)      -- Return folded string
                       same as icu_trans using the transform
      "Latin;NFKD;[:Nonspacing Mark:]Remove;Lower;NFKC;ascii"


Automatically create Collations:

Collation names with the form icu[_<locale>][_(AI|CI|AICI|CIAI)] will be
constructed automatically.  If no locale is specified then "und"
(undefined) will be used as the base locale.
eg: icu icu_aici icu_en icu_en_aici icu_fr_ca icu_fr_ca_aici


ks-level1   (Base)
ks-level2   (Accent)
ks-level3   (Case)
kx-level4
ks-identic

ka-ignore   (Alternative Weights)
ka-shifted

kb-true     (backwards)
kb-false

kk-true     (normalization)
kk-false

kc-true     (add level 2.5 case)
kc-false

kf-upper    (case first)
kf-lower
kf-false

kn-true     (numeric ordering)
kn-false


