TEMPLATE = aux

QMAKE_DOCS = doc.qdocconf

# FIXME: Refactor into load(qt_docs) or something similar
# that can be used from all non-module projects that also
# provide modularized docs, for example qmake.
QMAKE_DOCS_OUTPUTDIR = fmtdoc