import xlrd

xls_name = input("please input xls name:")
output_name = "xls.txt"

data = xlrd.open_workbook(xls_name)
table = data.sheets()[0]
nrows = table.nrows
ncols = table.ncols

with open(output_name, "w", encoding='utf-8') as f:
    f.write((str)(nrows))
    f.write(" ")
    f.write((str)(ncols))
    f.write("\n")