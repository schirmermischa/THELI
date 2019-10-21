#include "splitter.h"
#include "../instrumentdata.h"
#include "../functions.h"

#include <QString>
#include <QStringList>
#include <QVector>
#include <QFile>
#include <QDir>

// These routines do not query the FITS header itself, but rather the QStringList version

// Retrieve the key and update the output header
bool Splitter::searchKey(const QString &searchKeyName, const QStringList &possibleKeyNames, QStringList &outputHeader)
{
    bool inPrimaryHeader = false;
    bool inExtHeader = false;
    inPrimaryHeader = searchKeyInHeader(searchKeyName, possibleKeyNames, primaryHeader, outputHeader);
    if (!inPrimaryHeader) inExtHeader = searchKeyInHeader(searchKeyName, possibleKeyNames, extHeader, outputHeader);

    if (!inExtHeader && !inPrimaryHeader) return false;
    else return true;
}

// Retrieve the floating point value of a key, don't update output header
void Splitter::searchKeyCRVAL(const QString searchKey, const QStringList &possibleKeyNames, QStringList &outputHeader)
{
    bool inPrimaryHeader = false;
    bool inExtHeader = false;
    inPrimaryHeader = searchKeyInHeaderCRVAL(searchKey, possibleKeyNames, primaryHeader, outputHeader);
    if (!inPrimaryHeader) inExtHeader = searchKeyInHeaderCRVAL(searchKey, possibleKeyNames, extHeader, outputHeader);

    // Fallback:
    if (!inPrimaryHeader && !inExtHeader) {
        QString card;
        if (searchKey == "CRVAL1") card = "CRVAL1  = 0.00001";  // != 0, safeguard against potentially still existing ancient bug I lost track of ...
        if (searchKey == "CRVAL2") card = "CRVAL2  = 0.0";
        card.resize(80, ' ');
        outputHeader.append(card);
    }
}

// Retrieve the floating point value of a key, don't update output header
bool Splitter::searchKeyLST(const QStringList &possibleKeyNames)
{
    bool inPrimaryHeader = false;
    bool inExtHeader = false;
    inPrimaryHeader = searchKeyInHeaderLST(possibleKeyNames, primaryHeader);
    if (!inPrimaryHeader) inExtHeader = searchKeyInHeaderLST(possibleKeyNames, extHeader);

    if (!inExtHeader && !inPrimaryHeader) return false;
    else return true;
}

// Retrieve the int value of a key, don't update output header
bool Splitter::searchKeyValue(const QStringList &possibleKeyNames, int &value)
{
    bool inPrimaryHeader = false;
    bool inExtHeader = false;
    inPrimaryHeader = searchKeyInHeaderValue(possibleKeyNames, primaryHeader, value);
    if (!inPrimaryHeader) inExtHeader = searchKeyInHeaderValue(possibleKeyNames, extHeader, value);

    if (!inExtHeader && !inPrimaryHeader) return false;
    else return true;
}

// Retrieve the floating point value of a key, don't update output header
bool Splitter::searchKeyValue(const QStringList &possibleKeyNames, float &value)
{
    bool inPrimaryHeader = false;
    bool inExtHeader = false;
    inPrimaryHeader = searchKeyInHeaderValue(possibleKeyNames, primaryHeader, value);
    if (!inPrimaryHeader) inExtHeader = searchKeyInHeaderValue(possibleKeyNames, extHeader, value);

    if (!inExtHeader && !inPrimaryHeader) return false;
    else return true;
}

// Retrieve the double value of a key, don't update output header
bool Splitter::searchKeyValue(const QStringList &possibleKeyNames, double &value)
{
    bool inPrimaryHeader = false;
    bool inExtHeader = false;
    inPrimaryHeader = searchKeyInHeaderValue(possibleKeyNames, primaryHeader, value);
    if (!inPrimaryHeader) inExtHeader = searchKeyInHeaderValue(possibleKeyNames, extHeader, value);

    if (!inExtHeader && !inPrimaryHeader) return false;
    else return true;
}

// Retrieve the QString value of a key, don't update output header
bool Splitter::searchKeyValue(const QStringList &possibleKeyNames, QString &value)
{
    bool inPrimaryHeader = false;
    bool inExtHeader = false;
    inPrimaryHeader = searchKeyInHeaderValue(possibleKeyNames, primaryHeader, value);
    if (!inPrimaryHeader) inExtHeader = searchKeyInHeaderValue(possibleKeyNames, extHeader, value);

    if (!inExtHeader && !inPrimaryHeader) return false;
    else return true;
}

bool Splitter::searchKeyInHeader(const QString &searchKey, const QStringList &possibleKeyNames,
                                 const QStringList &inputHeader, QStringList &outputHeader)
{
    // TODO: enforce that numeric values are numeric, and string values have single quotes
    // some raw data don't obey this rule

    // TODO: preference to key in EXT if also found in PHDU, or the other way round?
    // Right now it will take the PHDU value if present there (because PHDU is searched first)

    bool keyFound = false;
    // Loop over list of possible key names to find match
    for (auto &possibleKey : possibleKeyNames) {
        // Loop over header
        for (auto &card : inputHeader) {
            QString keyName = card.split("=")[0].simplified();
            if (keyName == possibleKey) {
                QString keyValue = card.split("=")[1];
                QString newCard = searchKey;
                newCard.resize(8, ' ');          // Keyword must be 8 chars long
                newCard.append("= "+keyValue);
                newCard.resize(80, ' ');         // Header card must be 80 chars long
                outputHeader.append(newCard);
                keyFound = true;
                int slashPosition = keyValue.lastIndexOf('/');  // (can be -1 if not found, treated as 0 by truncate(), i.e. entire string is set to empty
                if (searchKey == "DATE-OBS") {
                    if (slashPosition > 12) keyValue.truncate(slashPosition);
                    keyValue.remove("'");
                    dateObsValue = keyValue.simplified();
                }
                if (searchKey == "MJD-OBS") {
                    if (slashPosition > 12) keyValue.truncate(slashPosition);
                    keyValue.remove("'");
                    mjdobsValue = keyValue.simplified().toDouble();
                }
                break;
            }
        }
        if (keyFound) break;
    }

    return keyFound;
}

bool Splitter::searchKeyInHeaderCRVAL(const QString searchKey, const QStringList &possibleKeyNames,
                                      const QStringList &inputHeader, QStringList &outputHeader)
{
    bool keyFound = false;
    // Loop over list of possible key names to find match
    for (auto &possibleKey : possibleKeyNames) {
        // Loop over header
        for (auto &card : inputHeader) {
            QString keyName = card.split("=")[0].simplified();
            if (keyName == possibleKey) {
                QString keyValue = card.split("=")[1];
                // Isolate CRVAL value
                int slashPosition = keyValue.lastIndexOf('/');
                if (slashPosition > 12) keyValue.truncate(slashPosition);
                keyValue.remove("'");
                keyValue = keyValue.simplified();
                // Fix format (sometimes we have 'HH MM SS' instead of 'HH:MM:SS')
                keyValue.replace(' ', ':');
                // Convert to decimal format if necessary
                QString crval = keyValue;
                if (keyValue.contains(':')) {
                    if (searchKey == "CRVAL1") crval = hmsToDecimal(keyValue);
                    if (searchKey == "CRVAL2") crval = dmsToDecimal(keyValue);
                }
                // Some instruments store CRVAL1 in decimal hours, oh well ...
                if (possibleKey == "RA-HOURS"
                        || instData.name == "SITe@TLS") {
                    crval = QString::number(15.*crval.toDouble(), 'f', 12);
                }

                // The William Herschel Telescope has its own funny "apertures" ...
                if (instData.name.contains("@WHT")) {
                    double ra_off1 = 0.;
                    double ra_off2 = 0.;
                    double dec_off1 = 0.;
                    double dec_off2 = 0.;
                    QStringList ra1 = {"XAPNOM"};
                    QStringList ra2 = {"XAPOFF"};
                    QStringList dec1 = {"YAPNOM"};
                    QStringList dec2 = {"YAPOFF"};
                    searchKeyInHeaderValue(ra1, inputHeader, ra_off1);
                    searchKeyInHeaderValue(ra2, inputHeader, ra_off2);
                    searchKeyInHeaderValue(dec1, inputHeader, dec_off1);
                    searchKeyInHeaderValue(dec2, inputHeader, dec_off2);
                    if (searchKey == "CRVAL1") crval = QString::number(crval.toDouble() + ra_off1 + ra_off2, 'f', 12);
                    if (searchKey == "CRVAL2") crval = QString::number(crval.toDouble() + dec_off1 + dec_off2, 'f', 12);
                }

                // Construct the key card
                QString newCard = searchKey;
                newCard.resize(8, ' ');          // Keyword must be 8 chars long
                newCard.append("= "+crval);
                newCard.resize(80, ' ');         // Header card must be 80 chars long
                outputHeader.append(newCard);
                keyFound = true;
                break;
            }
        }
        if (keyFound) break;
    }

    return keyFound;
}

bool Splitter::searchKeyInHeaderLST(const QStringList &possibleKeyNames, const QStringList &inputHeader)
{
    bool keyFound = false;
    // Loop over list of possible key names to find match
    for (auto &possibleKey : possibleKeyNames) {
        // Loop over header
        for (auto &card : inputHeader) {
            QString keyName = card.split("=")[0].simplified();
            if (keyName == possibleKey) {
                QString keyValue = card.split("=")[1];
                // Isolate LST value
                int slashPosition = keyValue.lastIndexOf('/');
                if (slashPosition > 12) keyValue.truncate(slashPosition);
                keyValue.remove("'");
                keyValue = keyValue.simplified();
                // Fix format (sometimes we have 'HH MM SS' instead of 'HH:MM:SS')
                keyValue.replace(' ', ':');
                // Convert to decimal format (decimal hours and then to seconds)
                // So far, I never found different units
                // TODO: check PANIC@LCO and CFH12K99@CFHT
                QString lst = keyValue;
                if (keyValue.contains(':')) {
                    lst = dmsToDecimal(keyValue);    // use dmstodecimal instead of hmstodecimal because we need LST in seconds
                }
                lstValue = 3600.*lst.toDouble();
                keyFound = true;
                break;
            }
        }
        if (keyFound) break;
    }

    return keyFound;
}

// int version
bool Splitter::searchKeyInHeaderValue(const QStringList &possibleKeyNames, const QStringList &inputHeader, int &value)
{
    bool keyFound = false;
    // Loop over possible keywords
    for (auto &possibleKey : possibleKeyNames) {
        // Loop over header
        for (auto &card : inputHeader) {
            QString keyName = card.split("=")[0].simplified();
            // Loop over list of possible key names to find match
            if (keyName == possibleKey) {
                QString keyValue = card.split("=")[1];
                // We are looking for a numeric value: remove quotes, simplify white space
                keyValue.remove("'");
                keyValue = keyValue.simplified();
                // Split (/, and potential comment), take the first element
                value = keyValue.split(' ').at(0).toInt();
                keyFound = true;
                break;
            }
        }
        if (keyFound) break;
    }
    return keyFound;
}

// float version
bool Splitter::searchKeyInHeaderValue(const QStringList &possibleKeyNames, const QStringList &inputHeader, float &value)
{
    bool keyFound = false;
    // Loop over possible keywords
    for (auto &possibleKey : possibleKeyNames) {
        // Loop over header
        for (auto &card : inputHeader) {
            QString keyName = card.split("=")[0].simplified();
            // Loop over list of possible key names to find match
            if (keyName == possibleKey) {
                QString keyValue = card.split("=")[1];
                // We are looking for a numeric value: remove quotes, simplify white space
                keyValue.remove("'");
                keyValue = keyValue.simplified();
                // Split (/, and potential comment), take the first element
                value = keyValue.split(' ').at(0).toFloat();
                keyFound = true;
                break;
            }
        }
        if (keyFound) break;
    }
    return keyFound;
}

// double version
bool Splitter::searchKeyInHeaderValue(const QStringList &possibleKeyNames, const QStringList &inputHeader, double &value)
{
    bool keyFound = false;
    // Loop over possible keywords
    for (auto &possibleKey : possibleKeyNames) {
        // Loop over header
        for (auto &card : inputHeader) {
            QString keyName = card.split("=")[0].simplified();
            // Loop over list of possible key names to find match
            if (keyName == possibleKey) {
                QString keyValue = card.split("=")[1];
                // We are looking for a numeric value: remove quotes, simplify white space
                keyValue.remove("'");
                keyValue = keyValue.simplified();
                // Split (/, and potential comment), take the first element
                value = keyValue.split(' ').at(0).toDouble();
                keyFound = true;
                break;
            }
        }
        if (keyFound) break;
    }
    return keyFound;
}

// QString version
bool Splitter::searchKeyInHeaderValue(const QStringList &possibleKeyNames, const QStringList &inputHeader, QString &value)
{
    bool keyFound = false;
    // Loop over possible keywords
    // WARNING: Assume we look for DATE-OBS, and the header contains both DATE-OBS and DATE keywords.
    // DATE-OBS is found first and is assumed to be correct (compared to DATE)
    // If we loop first over the header, DATE would be taken if it appears first in the header
    for (auto &possibleKey : possibleKeyNames) {
        // Loop over header to find a match
        for (auto &card : inputHeader) {
            QString keyName = card.split("=")[0].simplified();
            if (keyName == possibleKey) {
                QString keyValue = card.split("=")[1];
                int slashPosition = keyValue.lastIndexOf('/');
                // TODO: the slash might occur further in front! In particular for HIERARCH ESO cards
                if (slashPosition > 12) keyValue.truncate(slashPosition);
                keyValue.replace('/', '_');      // Some filter names contain a slash
                keyValue.remove("'");                          // Remove potential quotes from string value
                keyValue = keyValue.simplified();
                value = keyValue;
                keyFound = true;
                break;
            }
        }
        if (keyFound) break;
    }
    return keyFound;
}
