--системная организация.
--для служебного использования
/*
insert into organization (
                          id_type_org, 
                          id_curr_mode, 
                          id_prev_mode, 
                          name, 
                          short_name,
                          code_name, 
                          email_prefix, 
                          latitude, 
                          longitude, 
                          map_symbol, 
                          tree_symbol, 
                          dt_prev_mode, 
                          dt_curr_mode
                         )
                   values(
                          1, 
                          1,
                          1,
                          'DUMMY ORGANIZATION',
                          'DUMMY ORGANIZATION',
                          'DUMMY ORGANIZATION',
                          'localorg',
                          0, 
                          0,
                          '',
                          '',
                          current_timestamp,
                          current_timestamp
                         );
                         
*/