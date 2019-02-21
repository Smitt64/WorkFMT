var table = pTable.toFmtTable(); 
Print("Таблица:", table.Name, ". ", table.Comment);
for (var i = 0; i < table.fieldsCount(); i++) 
{ 
    var fld = table.field(i); 
    Print(fld.Name, ": ", fld.Comment); 
} 
Print("");
Print("Индексы:");
for (var i = 0; i < table.indexesCount(); i++) 
{
    var index = table.tableIndex(i);
    Print(" ", index.Name); 
    Print("  IndexNumber:", index.IndexNumber, ", SegmentsCount: ", 
        index.SegmentsCount, ", isAutoInc: ", index.isAutoInc, 
        ", isDup: ", index.isDup, ", isLocal: ", index.isLocal, ", isUnique: ", index.isUnique);
    
    for (var j = 0; j < index.segmentsCount(); j++) 
    {
        var segment = index.segment(j);
        Print("   ", segment.field().Name); 
    }
}
