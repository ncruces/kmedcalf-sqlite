
int core_init(const char* dummy)
{
    int nErr = 0;

    nErr += sqlite3_auto_extension((void*)sqlite3_sqlautobusy_init);        // Keith Medcalf code
    nErr += sqlite3_auto_extension((void*)sqlite3_base64_init);             // Keith Medcalf code
    nErr += sqlite3_auto_extension((void*)sqlite3_base85_init);             // Larry Brasfield code
    nErr += sqlite3_auto_extension((void*)sqlite3_define_init);             // Keith Medcalf code
    nErr += sqlite3_auto_extension((void*)sqlite3_ipaddress_init);          // Keith Medcalf code
    nErr += sqlite3_auto_extension((void*)sqlite3_pstoken_init);            // THird-party extension
    nErr += sqlite3_auto_extension((void*)sqlite3_sqlatof_init);            // Keith Medcalf code
    nErr += sqlite3_auto_extension((void*)sqlite3_sqlfcmp_init);            // Keith Medcalf code
    nErr += sqlite3_auto_extension((void*)sqlite3_sqlfunc_init);            // Keith Medcalf code
    nErr += sqlite3_auto_extension((void*)sqlite3_sqlfwin_init);            // Keith Medcalf code
    nErr += sqlite3_auto_extension((void*)sqlite3_sqlhash_init);            // Keith Medcalf code
    nErr += sqlite3_auto_extension((void*)sqlite3_sqlitemprint_init);       // Third-party extension
    nErr += sqlite3_auto_extension((void*)sqlite3_sqlmath_init);            // Keith Medcalf code
    nErr += sqlite3_auto_extension((void*)sqlite3_sqlmathtest_init);        // Keith Medcalf code
    nErr += sqlite3_auto_extension((void*)sqlite3_sqlnframes_init);         // Keith Medcalf code
    nErr += sqlite3_auto_extension((void*)sqlite3_sqlnumeric_init);         // Keith Medcalf code
    nErr += sqlite3_auto_extension((void*)sqlite3_sqlserialize_init);       // Keith Medcalf code
    nErr += sqlite3_auto_extension((void*)sqlite3_sqlsize_init);            // Keith Medcalf code
    nErr += sqlite3_auto_extension((void*)sqlite3_sqltext_init);            // Keith Medcalf code
    nErr += sqlite3_auto_extension((void*)sqlite3_sqltime_init);            // Keith Medcalf code
//  nErr += sqlite3_auto_extension((void*)sqlite3_sqlunhex_init);           // Keith Medcalf code * NOW IN CORE DISTRIBUTION *
    nErr += sqlite3_auto_extension((void*)sqlite3_uuid_init);               // Keith Medcalf code

    nErr += sqlite3_auto_extension((void*)sqlite3_blobio_init);             // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_decimal_init);            // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_eval_init);               // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_fileio_init);             // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_ieee_init);               // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_nextchar_init);           // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_percentile_init);         // SQLite3 misc extension

#ifndef SQLITE_ENABLE_ICU
    nErr += sqlite3_auto_extension((void*)sqlite3_regexp_init);             // SQLite3 misc extension
#endif

    nErr += sqlite3_auto_extension((void*)sqlite3_rot_init);                // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_totype_init);             // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_uint_init);               // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_urifuncs_init);           // SQLite3 misc extebsion
    nErr += sqlite3_auto_extension((void*)sqlite3_zorder_init);             // SQLite3 misc extension

    nErr += sqlite3_auto_extension((void*)sqlite3_fossildelta_init);        // SQLite3 misc extension

#ifndef SQLITE_OMIT_VIRTUALTABLE
    nErr += sqlite3_auto_extension((void*)sqlite3_amatch_init);             // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_btreeinfo_init);          // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_carray_init);             // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_closure_init);            // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_completion_init);         // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_csv_init);                // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_explain_init);            // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_fuzzer_init);             // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_memstat_init);            // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_pivotvtab_init);          // Third-Party extension
    nErr += sqlite3_auto_extension((void*)sqlite3_prefixes_init);           // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_series_init);             // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_spellfix_init);           // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_statementvtab_init);      // Third-Party extension
    nErr += sqlite3_auto_extension((void*)sqlite3_unionvtab_init);          // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_vtshim_init);             // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_wholenumber_init);        // SQLite3 misc extension
#ifdef SQLITE_ENABLE_VFSSTAT
    nErr += sqlite3_vfsstat_init((void*)dummy, (void*)0, (void*)0);         // SQLite3 misc extension
#endif
    nErr += sqlite3_auto_extension((void*)sqlite3_vsv_init);                // Keith Medcalf code
#ifndef _MSC_VER
    nErr += sqlite3_auto_extension((void*)sqlite3_interpolate_init);        // Third-party extension
#endif
#endif

#ifdef SQLITE_HAVE_ZLIB
    nErr += sqlite3_auto_extension((void*)sqlite3_compress_init);           // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_zipfile_init);            // SQLite3 misc extension
    nErr += sqlite3_auto_extension((void*)sqlite3_sqlar_init);              // SQLite3 misc extension
#endif

#ifdef SQLITE_ENABLE_LSM1
    nErr += sqlite3_auto_extension((void*)sqlite3_lsm_init);                // SQLite3 lsm1 extension
#endif

#ifdef SQLITE_USE_CKSUMVFS
    nErr += sqlite3_register_cksumvfs(0);
#endif

    return nErr ? SQLITE_ERROR : SQLITE_OK;
}

