# Assumptions: test schema assigned to db, my_collection collection exists

# Gets the collection
myColl = db.getCollection('my_collection')

# Insert a document
res = myColl.add({ 'name': 'Jack', 'age': 15, 'height': 1.76 }).execute()

# Print the documentId that was assigned to the document
print 'Document Id: %s\n' % res.getLastDocumentId()