import os
from flask import Flask, render_template, request

app = Flask(__name__)


@app.route('/')
def welcomePage():
    return render_template("index.html")

@app.route('/run/', methods=["POST", "GET"])
def run():
    if request.method == "POST":
        id = request.form["partyId"]
        nOfSeq = request.form["nOfSeq"]
        
        """
        Run SMC
        """
        cmd = './runUPGMA ' + id + ' ' + nOfSeq
        os.system(cmd)

        tree_raw = open("phylogeneticTree/upgma_tree.nwk", "r")
        tree_file = tree_raw.read()

        return render_template("smcComplete.html", tree=tree_file)
    else:
        return render_template("runPrivPhyloTree.html")



@app.route('/runUPGMA')
def runUPGMA():

    cmd = './runUPGMA 0 2'
    os.system(cmd)

    return "Computing..."




if __name__ == '__main__':
    app.run(debug=True)