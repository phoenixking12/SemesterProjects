import pandas as pd
from sklearn.linear_model import LinearRegression

train_file = "train.csv"
dataframe = pd.read_csv(train_file)

X = dataframe[['LotArea', 'BedroomAbvGr', 'FullBath', 'HalfBath']]
y = dataframe['SalePrice']

model = LinearRegression()
model.fit(X, y)


test_file = "test.csv"
test_data = pd.read_csv(test_file)
X_test = test_data[['LotArea', 'BedroomAbvGr', 'FullBath', 'HalfBath']]
test_predictions = model.predict(X_test)

submission = pd.DataFrame({'Id': test_data['Id'], 'LotArea':test_data['LotArea'],'BedroomAbvGr':test_data['BedroomAbvGr'],
                           'FullBath':test_data['FullBath'],'HalfBath':test_data['HalfBath'],'SalePrice': test_predictions})
submission.to_csv("submission.csv", index=False)
print("results predicted")