// Copyright (c) 2011-2020 The Blokecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_BITCOINADDRESSVALIDATOR_H
#define BITCOIN_QT_BITCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class BlokecoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BlokecoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Blokecoin address widget validator, checks for a valid blokecoin address.
 */
class BlokecoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BlokecoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // BITCOIN_QT_BITCOINADDRESSVALIDATOR_H
