select DECODE(rownum, 1, ' ORDER BY ', ',') || 
             DECODE(descend, 'ASC', column_name, '') col, 
             column_expression, DECODE(descend, 'DESC', ' desc', '') 
      from ( 
       select descend, c.column_name, e.column_expression 
       from user_ind_columns c, user_ind_expressions e 
        where c.index_name = e.index_name (+) 
          and c.column_position = e.column_position (+) 
          and c.index_name = ( 
      select DECODE( 
        (select index_name from user_constraints 
          where table_name = '%1' and (constraint_type = 'P' or constraint_type = 'U') 
            and constraint_type = ( 
                select min(constraint_type) from user_constraints 
                  where table_name = '%1' and (constraint_type = 'P' or constraint_type = 'U'))), '', 
        (select min(index_name) from user_indexes 
          where uniqueness = 'UNIQUE' 
            and table_name = '%1'),(select index_name from user_constraints 
          where table_name = '%1' and (constraint_type = 'P' or constraint_type = 'U') 
            and constraint_type = ( 
                select min(constraint_type) from user_constraints 
                  where table_name = '%1' and (constraint_type = 'P' or constraint_type = 'U')))) 
        from dual) 
      order by c.column_position)