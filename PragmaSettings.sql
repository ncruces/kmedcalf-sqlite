drop view if exists PragmaSettings;
create view if not exists temp.PragmaSettings(schema, pragma, setting) as
    with param(srcschema, ro, seq) as
         (
            select name,
                   ro,
                   seq
              from pragma_database_list
         ),
         settings(pragma, schema, seq, setting) as
         (
              select 'auto_vacuum',
                     srcschema,
                     seq,
                     auto_vacuum
                from pragma_auto_vacuum , param
               where schema=srcschema
          union all
              select 'cache_size',
                     srcschema,
                     seq,
                     cache_size
                from pragma_cache_size , param
               where schema=srcschema
          union all
              select 'cache_spill',
                     srcschema,
                     seq,
                     cache_spill
                from pragma_cache_spill , param
               where schema=srcschema
          union all
              select 'journal_mode',
                     srcschema,
                     seq,
                     journal_mode
                from pragma_journal_mode , param
               where schema=srcschema
          union all
              select 'journal_size_limit',
                     srcschema,
                     seq,
                     journal_size_limit
                from pragma_journal_size_limit , param
               where schema=srcschema
          union all
              select 'locking_mode',
                     srcschema,
                     seq,
                     locking_mode
                from pragma_locking_mode , param
               where schema=srcschema
          union all
              select 'max_page_count',
                     srcschema,
                     seq,
                     max_page_count
                from pragma_max_page_count , param
               where schema=srcschema
          union all
              select 'read_only',
                     srcschema,
                     seq,
                     ro
                from param
          union all
              select 'secure_delete',
                     srcschema,
                     seq,
                     secure_delete
                from pragma_secure_delete , param
               where schema=srcschema
          union all
              select 'synchronous',
                     srcschema,
                     seq,
                     synchronous
                from pragma_synchronous , param
               where schema=srcschema
          union all
              select 'application_id',
                     srcschema,
                     seq,
                     application_id
                from pragma_application_id , param
               where schema=srcschema
          union all
              select 'user_version',
                     srcschema,
                     seq,
                     user_version
                from pragma_user_version , param
               where schema=srcschema

          union all
              select 'analysis_limit',
                     '*',
                     9999,
                     analysis_limit
                from pragma_analysis_limit
          union all
              select 'automatic_index',
                     '*',
                     9999,
                     automatic_index
                from pragma_automatic_index
          union all
              select 'busy_timeout',
                     '*',
                     9999,
                     timeout
                from pragma_busy_timeout
          union all
              select 'cell_size_check',
                     '*',
                     9999,
                     cell_size_check
                from pragma_cell_size_check
          union all
              select 'checkpoint_fullfsync',
                     '*',
                     9999,
                     checkpoint_fullfsync
                from pragma_checkpoint_fullfsync
          union all
              select 'defer_foreign_keys',
                     '*',
                     9999,
                     defer_foreign_keys
                from pragma_defer_foreign_keys
          union all
              select 'foreign_keys',
                     '*',
                     9999,
                     foreign_keys
                from pragma_foreign_keys
          union all
              select 'fullfsync',
                     '*',
                     9999,
                     fullfsync
                from pragma_fullfsync
          union all
              select 'hard_heap_limit',
                     '*',
                     9999,
                     hard_heap_limit
                from pragma_hard_heap_limit
          union all
              select 'ignore_check_constraints',
                     '*',
                     9999,
                     ignore_check_constraints
                from pragma_ignore_check_constraints
          union all
              select 'legacy_alter_table',
                     '*',
                     9999,
                     legacy_alter_table
                from pragma_legacy_alter_table
          union all
              select 'query_only',
                     '*',
                     9999,
                     query_only
                from pragma_query_only
          union all
              select 'read_uncommitted',
                     '*',
                     9999,
                     read_uncommitted
                from pragma_read_uncommitted
          union all
              select 'recursive_triggers',
                     '*',
                     9999,
                     recursive_triggers
                from pragma_recursive_triggers
          union all
              select 'reverse_unordered_selects',
                     '*',
                     9999,
                     reverse_unordered_selects
                from pragma_reverse_unordered_selects
          union all
              select 'soft_heap_limit',
                     '*',
                     9999,
                     soft_heap_limit
                from pragma_soft_heap_limit
          union all
              select 'temp_store',
                     '*',
                     9999,
                     temp_store
                from pragma_temp_store
          union all
              select 'threads',
                     '*',
                     9999,
                     threads
                from pragma_threads
          union all
              select 'trusted_schema',
                     '*',
                     9999,
                     trusted_schema
                from pragma_trusted_schema
          union all
              select 'writable_schema',
                     '*',
                     9999,
                     writable_schema
                from pragma_writable_schema
         )
    select schema,
           pragma,
           setting
      from settings
  order by seq, pragma
;
