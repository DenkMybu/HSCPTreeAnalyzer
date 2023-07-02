
# check if a line or a work is commented
# meaning starting by a #
def IsCommented(label):
    return label.split()[0][0]=="#"

# Generate the prototype of a data function used for HSPC selection in the HSCPSelector class
# A label should be associated to the selection
def HSCPSelectorProto(label):
    #content for header file
    hcontent = "bool PassHSCPpresel_"+label+"(int hscpIndex);\n"
    return hcontent 

# Generate a data function implementation used for HSPC selection in the HSCPSelector class
# A label should be associated to the selection
# Assume that instruction are already c++ instructions using TTreeReadValues or Arrays
def HSCPSelectorImpl(label, instruction):
    #content for C file
    content = "bool HSCPSelector::PassHSCPpresel_"+label+"(int i){\n"
    content+= "   if (i<0 || i>(int)Pt.GetSize()) return false;\n"

    #ADDED HERE
    #content +="   bool tmp = " + instruction+";\n"

    #####

    #content+= "   "+.replace('[i]','->at(hscpIndex)')

    #OLD HERE  
    content+= "   return "+instruction+";\n"
    #content+='   std::cout << "bool for presel = " << tmp << endl;\n'
    #content+= "   return tmp;\n"
    content+= "}\n"
    return content

# Generation lines of code to add the method and the label into vectors
def HSCPSelectorAddLabelsAndPointers2Vector(label):
   content = "selections_.push_back(&HSCPSelector::PassHSCPpresel_"+label+");\n"
   content+= "selLabels_.push_back(\""+label+"\");\n" 
   return content


def Code_AddPlot(df):
    code = ""
    for index, row in df.iterrows():
        if IsCommented(row[0]): 
            continue
        code+="plots.AddHisto1D(selLabels_[i]+\""+row['label']+"\","+str(row['nbins'])+","+str(row['min'])+","+str(row['max'])+");\n"
    return code

def Code_FillPlot(df):
    code = ""
    for index, row in df.iterrows():
        if IsCommented(row[0]): 
            continue
        code+="vcp[s].FillHisto1D(selLabels_[s]+\""+row['label']+"\","+row['variable']+"[i]);\n"
    return code


# The 3 functions below generate the codes for all preselection found in a pandas dataframe called pd

def Code_HSCPSelectorProto(df):
    code = ""
    for index, row in df.iterrows():
        if IsCommented(row[0]): 
            continue
        code+=HSCPSelectorProto(row[0])+"\n"
    return code

def Code_HSCPSelectorImpl(df):
    code = ""
    for index, row in df.iterrows():
        if IsCommented(row[0]): continue
        code+=HSCPSelectorImpl(row[0],row[1])+"\n"
    return code

def Code_HSCPSelectorAddLabelsAndPointers2Vector(df):
    code = ""
    for index, row in df.iterrows():
        if IsCommented(row[0]): continue
        code+=HSCPSelectorAddLabelsAndPointers2Vector(row[0])+"\n"
    return code



