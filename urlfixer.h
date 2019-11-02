#ifndef URLFIXER_H
#define URLFIXER_H

class UrlFixer {
private:
public:
    UrlFixer() {
    }

    bool hasClosures(QString url) {
        QCharRef e = url[url.length()-1];
        QCharRef b = url[0];

        if (b == '"' && e == '"')
            return true;
        if (b == '\'' && e == '\'')
            return true;
        if (b == '[' && e == ']')
            return true;
        if (b == '(' && e == ')')
            return true;
        if (b == '{' && e == '}')
            return true;
        return false;
    }

    QString fix(QString url) {
        register int pos, i;

        url.trimmed();
        int l = url.length();
        if (l < 2)
            return "";

        // remove closures
        if (hasClosures(url)) {
            l -= 2;
            url = url.mid(1,l);
        }

        // remove init tabs and spaces
        pos = 0;
        for (i=0; i<l; i++) {
            if (url[i] == ' ' || url[i] == '\t')
                pos +=1;
            else
                break;
        }
        if (pos>0) {
            l -= pos;
            url = url.mid(pos, l);
        }

        // remove spaces at the end
        for (i=l-1; i>=0; i--) {
            if (url[i] != ' ' && url[i] != '\t')
                break;
        }
        url = url.mid(0, l);

        // remove stuff before https:// or http://
        std::string surl = url.toStdString();
        pos = surl.find("http://");
        if (pos>0) {
            l -= pos;
            url = url.mid(pos, l);
        }
        pos = surl.find("https://");
        if (pos>0) {
            l -= pos;
            url = url.mid(pos, l);
        }

        // missing protocol
        if (!url.contains("://")) {
             url = "http://"+url;
        }

        return url;
    }

};


#endif // URLFIXER_H
