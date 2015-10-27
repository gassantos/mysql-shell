#@ mysqlx module: exports
|Exported Items: 23|
|getSession: <type 'builtin_function_or_method'>|
|getNodeSession: <type 'builtin_function_or_method'>|
|expr: <type 'builtin_function_or_method'>|
|Blob: <type 'builtin_function_or_method'>|
|Text: <type 'builtin_function_or_method'>|
|Decimal: <type 'builtin_function_or_method'>|
|Numeric: <type 'builtin_function_or_method'>|
|<DataTypes.TinyInt>|
|<DataTypes.SmallInt>|
|<DataTypes.MediumInt>|
|<DataTypes.Int>|
|<DataTypes.Integer>|
|<DataTypes.BigInt>|
|<DataTypes.Real>|
|<DataTypes.Float>|
|<DataTypes.Double>|
|<DataTypes.Date>|
|<DataTypes.Time>|
|<DataTypes.Timestamp>|
|<DataTypes.DateTime>|
|<DataTypes.Year>|
|<DataTypes.Bit>|
|<DataTypes.Blob(10)>|
|<DataTypes.Text(10)>|
|<IndexTypes.IndexUnique>|
|<DataTypes.Decimal>|
|<DataTypes.Decimal(10)>|
|<DataTypes.Decimal(10,3)>|
|<DataTypes.Numeric>|
|<DataTypes.Numeric(10)>|
|<DataTypes.Numeric(10,3)>|

#@ mysqlx module: getSession through URI
|<XSession:|
|Session using right URI|

#@ mysqlx module: getSession through URI and password
|<XSession:|
|Session using right URI|

#@ mysqlx module: getSession through data
|<XSession:|
|Session using right URI|

#@ mysqlx module: getSession through data and password
|<XSession:|
|Session using right URI|


#@ mysqlx module: getNodeSession through URI
|<NodeSession:|
|Session using right URI|

#@ mysqlx module: getNodeSession through URI and password
|<NodeSession:|
|Session using right URI|

#@ mysqlx module: getNodeSession through data
|<NodeSession:|
|Session using right URI|

#@ mysqlx module: getNodeSession through data and password
|<NodeSession:|
|Session using right URI|

#@ Stored Sessions, session from data dictionary
|<XSession:|
|Session using right URI|

#@ Stored Sessions, session from data dictionary removed
||IndexError: unknown attribute: mysqlx_data

#@ Stored Sessions, session from uri
|<XSession:|
|Session using right URI|

#@ Stored Sessions, session from uri removed
||IndexError: unknown attribute: mysqlx_uri


#@# mysqlx module: expression errors
||Invalid number of arguments in mysqlx.expr, expected 1 but got 0
||mysqlx.expr: Argument #1 is expected to be a string

#@ mysqlx module: expression
|<Expression>|