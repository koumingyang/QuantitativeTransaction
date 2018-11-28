import xlrd

xls_name = input("please input xls name:")
output_name = "xls.txt"

data = xlrd.open_workbook(xls_name)
table = data.sheets()[0]
nrows = table.nrows
ncols = table.ncols
all_rows = 0

NAN = (str)(1e50)


for i in range(1, nrows):
    val = (str)(table.cell(i, 4).value)
    if val != "NaN":
        all_rows += 1

with open(output_name, "w", encoding='utf-8') as f:
    f.write((str)(all_rows))
    f.write(" ")
    f.write((str)(ncols))
    f.write("\n")

    for i in range(1, nrows):
        val = (str)(table.cell(i, 4).value)
        if val != "NaN":
            for j in range(0, ncols):
                val = (str)(table.cell(i, j).value)
                if val == "NaN":
                    f.write(NAN)
                else:
                    f.write(val)
                f.write(" ")
            f.write("\n")
