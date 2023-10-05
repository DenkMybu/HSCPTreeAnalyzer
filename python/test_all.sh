#!/bin/bash

./make_pred_only_rootfiles.sh no 0
./draw_preds.sh
python DrawToyCRDataObs.py
./draw_signals.sh
