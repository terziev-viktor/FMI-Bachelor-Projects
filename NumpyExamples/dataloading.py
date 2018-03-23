import pandas

X = pandas.read_csv('Sacramentorealestatetransactions.csv') # reads .csv data file and returns a matrix like object
M = X.as_matrix() # converts to numpy array (not a regular python list of lists)

print(type(X)) # type of pandas method's return object (pandas.core.frame.DataFrame)
print(X.info()) # info for the object
print(X.head(10)) # preview of the dataset
print(X.columns) # returns a list of all column names in the loaded dataframe, you can change the by assingning a new list to x.clolumns
print(type(M)) # type of .as_matrix() return value (numpy.ndarray)
print(M)

print(X['street']) # passing the name of the column to X[] returns the whole column
print(X.street) # same as above, works only if column names are 'strings'
print(type(X['street'])) # pandas.core.series.Series is the 1D object data type of pandas
print(X.iloc[0]) # print a row, the type is also pandas.core.series.Series

# the apply() function - assingning values to rows based on other data
def get_bedsandbaths(row):
    return row.beds + row.baths

X['beds_and_baths'] = X.apply(get_bedsandbaths, axis=1) # apply the new column to each row (axis = 1 means apply to row)

print(X.head(10)) # lets see if the new column was applied
