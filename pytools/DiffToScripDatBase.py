import glob, codecs, sqlite3
from pathlib import Path
dat_path = 'd:\\svn\\UranRSBankV6\\Main\\Distrib\\DBFile\\Data'

def readLines(lines):
    columns = []
    is_column = False
    for line in lines:
        try:
            if ((not is_column) and (line[0] == '(')):
                is_column = True
            
            if (is_column):
                col = line.replace('(', '').replace(',', '').rstrip()

            pos = col.find(' ')

            
            if (col[-1] == ')'):
                col = line.replace(')', '')
                is_column = False

            if pos != -1:
                col = col[:pos]

            if (not is_column):
                if col != '':
                    columns.append(col)
                break
            else:
                if col != '':
                    columns.append(col)
        except:
            pass
    
    return columns

conn = sqlite3.connect('datstruct.info')

sql_statements = [ 
        '''CREATE TABLE IF NOT EXISTS DAT_FIELDS (
                id INTEGER PRIMARY KEY, 
                NAME text NOT NULL, 
                column TEXT
        );''']

cursor = conn.cursor()
for statement in sql_statements:
    cursor.execute(statement)

index = 1
for name in glob.glob(dat_path + '//*.dat'): 
    with codecs.open(name, "r", "866") as f:
        lines = f.read().splitlines()
        cols = readLines(lines)

        if len(cols) != 0:
            for col in cols:
                cursor.execute('insert into DAT_FIELDS(id, NAME, column)values(?, ?, ?)', (index, Path(name).stem.upper(), col.upper()))
                index = index + 1

            print(Path(name).stem)
            print(cols)
            print('-------------------------------------------')

conn.commit()
cursor.close();
conn.close();