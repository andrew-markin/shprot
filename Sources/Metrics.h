#ifndef METRICS_H
#define METRICS_H

class QDialog;
class QDialogButtonBox;
class QVBoxLayout;

// Spacings, margins and sizes are given for the font they were designed with and grow with larger fonts

namespace Metrics {

int scaled(int value);

int dialogMargin();
int dialogSpacing();
int groupSpacing();
int titleSpacing();

// Places content above a button box, returns the box to add buttons to (their tab order then follows visual order)
QDialogButtonBox* setupDialogLayout(QDialog* dialog, QVBoxLayout* contentLayout);

} // namespace Metrics

#endif // METRICS_H
