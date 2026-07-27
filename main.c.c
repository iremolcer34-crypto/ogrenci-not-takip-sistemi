#include <stdio.h>
#include <string.h>

#define MAX_OGRENCI 100
#define DOSYA_ADI "ogrenciler.txt"

typedef struct {
    int numara;
    char ad[50];
    char soyad[50];
    float vize;
    float final;
    float ortalama;
    char harfNotu[3];
} Ogrenci;

Ogrenci ogrenciler[MAX_OGRENCI];
int ogrenciSayisi = 0;

void menuGoster();
void ogrenciEkle();
void ogrencileriListele();
void ogrenciAra();
void ogrenciSil();
void ogrenciGuncelle();
void siralamaYap();
void dosyayaKaydet();
void dosyadanOku();
void bufferTemizle();

float ortalamaHesapla(float vize, float final);
void harfNotuHesapla(float ortalama, char harfNotu[]);
int ogrenciBul(int numara);

int main() {
    int secim;

    dosyadanOku();

    do {
        menuGoster();
        printf("Seciminiz: ");

        if (scanf("%d", &secim) != 1) {
            printf("Hatali giris yaptiniz.\n\n");
            bufferTemizle();
            continue;
        }

        bufferTemizle();

        switch (secim) {
            case 1:
                ogrenciEkle();
                break;
            case 2:
                ogrencileriListele();
                break;
            case 3:
                ogrenciAra();
                break;
            case 4:
                ogrenciSil();
                break;
            case 5:
                ogrenciGuncelle();
                break;
            case 6:
                siralamaYap();
                break;
            case 7:
                dosyayaKaydet();
                printf("Bilgiler dosyaya kaydedildi.\n\n");
                break;
            case 0:
                dosyayaKaydet();
                printf("Program kapatildi.\n");
                break;
            default:
                printf("Gecersiz secim.\n\n");
        }

    } while (secim != 0);

    return 0;
}

void menuGoster() {
    printf("\n--- OGRENCI NOT TAKIP SISTEMI ---\n");
    printf("1. Ogrenci ekle\n");
    printf("2. Ogrencileri listele\n");
    printf("3. Ogrenci ara\n");
    printf("4. Ogrenci sil\n");
    printf("5. Ogrenci guncelle\n");
    printf("6. Ortalamaya gore sirala\n");
    printf("7. Dosyaya kaydet\n");
    printf("0. Cikis\n");
}

void ogrenciEkle() {
    Ogrenci yeniOgrenci;

    if (ogrenciSayisi == MAX_OGRENCI) {
        printf("Daha fazla ogrenci eklenemez.\n\n");
        return;
    }

    printf("Ogrenci numarasi: ");
    if (scanf("%d", &yeniOgrenci.numara) != 1) {
        printf("Numara hatali girildi.\n\n");
        bufferTemizle();
        return;
    }
    bufferTemizle();

    if (ogrenciBul(yeniOgrenci.numara) != -1) {
        printf("Bu numara zaten kayitli.\n\n");
        return;
    }

    printf("Ogrenci adi: ");
    fgets(yeniOgrenci.ad, 50, stdin);
    yeniOgrenci.ad[strcspn(yeniOgrenci.ad, "\n")] = '\0';

    printf("Ogrenci soyadi: ");
    fgets(yeniOgrenci.soyad, 50, stdin);
    yeniOgrenci.soyad[strcspn(yeniOgrenci.soyad, "\n")] = '\0';

    if (strlen(yeniOgrenci.ad) == 0 || strlen(yeniOgrenci.soyad) == 0) {
        printf("Ad ve soyad bos birakilamaz.\n\n");
        return;
    }

    do {
        printf("Vize notu: ");

        if (scanf("%f", &yeniOgrenci.vize) != 1) {
            printf("Sayisal bir not giriniz.\n");
            bufferTemizle();
            yeniOgrenci.vize = -1;
            continue;
        }

        if (yeniOgrenci.vize < 0 || yeniOgrenci.vize > 100) {
            printf("Not 0 ile 100 arasinda olmali.\n");
        }

    } while (yeniOgrenci.vize < 0 || yeniOgrenci.vize > 100);

    do {
        printf("Final notu: ");

        if (scanf("%f", &yeniOgrenci.final) != 1) {
            printf("Sayisal bir not giriniz.\n");
            bufferTemizle();
            yeniOgrenci.final = -1;
            continue;
        }

        if (yeniOgrenci.final < 0 || yeniOgrenci.final > 100) {
            printf("Not 0 ile 100 arasinda olmali.\n");
        }

    } while (yeniOgrenci.final < 0 || yeniOgrenci.final > 100);

    bufferTemizle();

    yeniOgrenci.ortalama = ortalamaHesapla(yeniOgrenci.vize, yeniOgrenci.final);
    harfNotuHesapla(yeniOgrenci.ortalama, yeniOgrenci.harfNotu);

    ogrenciler[ogrenciSayisi] = yeniOgrenci;
    ogrenciSayisi++;

    dosyayaKaydet();
    printf("Ogrenci eklendi.\n\n");
}

void ogrencileriListele() {
    int i;

    if (ogrenciSayisi == 0) {
        printf("Kayitli ogrenci yok.\n\n");
        return;
    }

    printf("\n--- OGRENCI LISTESI ---\n");

    for (i = 0; i < ogrenciSayisi; i++) {
        printf("\n%d. ogrenci\n", i + 1);
        printf("Numara     : %d\n", ogrenciler[i].numara);
        printf("Ad Soyad   : %s %s\n", ogrenciler[i].ad, ogrenciler[i].soyad);
        printf("Vize       : %.1f\n", ogrenciler[i].vize);
        printf("Final      : %.1f\n", ogrenciler[i].final);
        printf("Ortalama   : %.2f\n", ogrenciler[i].ortalama);
        printf("Harf Notu  : %s\n", ogrenciler[i].harfNotu);
    }

    printf("\nToplam ogrenci sayisi: %d\n\n", ogrenciSayisi);
}

void ogrenciAra() {
    int numara;
    int index;

    printf("Aranacak ogrencinin numarasi: ");

    if (scanf("%d", &numara) != 1) {
        printf("Hatali numara girdiniz.\n\n");
        bufferTemizle();
        return;
    }

    bufferTemizle();
    index = ogrenciBul(numara);

    if (index == -1) {
        printf("Ogrenci bulunamadi.\n\n");
        return;
    }

    printf("\nOgrenci bulundu.\n");
    printf("Numara: %d\n", ogrenciler[index].numara);
    printf("Ad Soyad: %s %s\n", ogrenciler[index].ad, ogrenciler[index].soyad);
    printf("Vize: %.1f\n", ogrenciler[index].vize);
    printf("Final: %.1f\n", ogrenciler[index].final);
    printf("Ortalama: %.2f\n", ogrenciler[index].ortalama);
    printf("Harf Notu: %s\n\n", ogrenciler[index].harfNotu);
}

void ogrenciSil() {
    int numara;
    int index;
    int i;
    char cevap;

    printf("Silinecek ogrencinin numarasi: ");

    if (scanf("%d", &numara) != 1) {
        printf("Hatali numara girdiniz.\n\n");
        bufferTemizle();
        return;
    }

    bufferTemizle();
    index = ogrenciBul(numara);

    if (index == -1) {
        printf("Ogrenci bulunamadi.\n\n");
        return;
    }

    printf("%s %s silinsin mi? (e/h): ", ogrenciler[index].ad, ogrenciler[index].soyad);
    scanf(" %c", &cevap);
    bufferTemizle();

    if (cevap != 'e' && cevap != 'E') {
        printf("Silme islemi iptal edildi.\n\n");
        return;
    }

    for (i = index; i < ogrenciSayisi - 1; i++) {
        ogrenciler[i] = ogrenciler[i + 1];
    }

    ogrenciSayisi--;
    dosyayaKaydet();

    printf("Ogrenci silindi.\n\n");
}

void ogrenciGuncelle() {
    int numara;
    int index;
    char yeniAd[50];
    char yeniSoyad[50];
    float yeniVize;
    float yeniFinal;

    printf("Guncellenecek ogrencinin numarasi: ");

    if (scanf("%d", &numara) != 1) {
        printf("Hatali numara girdiniz.\n\n");
        bufferTemizle();
        return;
    }

    bufferTemizle();
    index = ogrenciBul(numara);

    if (index == -1) {
        printf("Ogrenci bulunamadi.\n\n");
        return;
    }

    printf("Yeni ad: ");
    fgets(yeniAd, 50, stdin);
    yeniAd[strcspn(yeniAd, "\n")] = '\0';

    printf("Yeni soyad: ");
    fgets(yeniSoyad, 50, stdin);
    yeniSoyad[strcspn(yeniSoyad, "\n")] = '\0';

    if (strlen(yeniAd) == 0 || strlen(yeniSoyad) == 0) {
        printf("Ad ve soyad bos birakilamaz.\n\n");
        return;
    }

    do {
        printf("Yeni vize notu: ");

        if (scanf("%f", &yeniVize) != 1) {
            printf("Sayisal bir not giriniz.\n");
            bufferTemizle();
            yeniVize = -1;
        }

    } while (yeniVize < 0 || yeniVize > 100);

    do {
        printf("Yeni final notu: ");

        if (scanf("%f", &yeniFinal) != 1) {
            printf("Sayisal bir not giriniz.\n");
            bufferTemizle();
            yeniFinal = -1;
        }

    } while (yeniFinal < 0 || yeniFinal > 100);

    bufferTemizle();

    strcpy(ogrenciler[index].ad, yeniAd);
    strcpy(ogrenciler[index].soyad, yeniSoyad);
    ogrenciler[index].vize = yeniVize;
    ogrenciler[index].final = yeniFinal;
    ogrenciler[index].ortalama = ortalamaHesapla(yeniVize, yeniFinal);

    harfNotuHesapla(ogrenciler[index].ortalama, ogrenciler[index].harfNotu);

    dosyayaKaydet();
    printf("Ogrenci bilgileri guncellendi.\n\n");
}

void siralamaYap() {
    int i, j;
    Ogrenci gecici;

    if (ogrenciSayisi == 0) {
        printf("Siralanacak ogrenci yok.\n\n");
        return;
    }

    for (i = 0; i < ogrenciSayisi - 1; i++) {
        for (j = 0; j < ogrenciSayisi - 1 - i; j++) {
            if (ogrenciler[j].ortalama < ogrenciler[j + 1].ortalama) {
                gecici = ogrenciler[j];
                ogrenciler[j] = ogrenciler[j + 1];
                ogrenciler[j + 1] = gecici;
            }
        }
    }

    printf("Ogrenciler ortalamaya gore siralandi.\n");
    ogrencileriListele();
}

float ortalamaHesapla(float vize, float final) {
    float sonuc;
    sonuc = vize * 0.40f + final * 0.60f;
    return sonuc;
}

void harfNotuHesapla(float ortalama, char harfNotu[]) {
    if (ortalama >= 90) {
        strcpy(harfNotu, "AA");
    } else if (ortalama >= 85) {
        strcpy(harfNotu, "BA");
    } else if (ortalama >= 80) {
        strcpy(harfNotu, "BB");
    } else if (ortalama >= 75) {
        strcpy(harfNotu, "CB");
    } else if (ortalama >= 70) {
        strcpy(harfNotu, "CC");
    } else if (ortalama >= 65) {
        strcpy(harfNotu, "DC");
    } else if (ortalama >= 60) {
        strcpy(harfNotu, "DD");
    } else {
        strcpy(harfNotu, "FF");
    }
}

int ogrenciBul(int numara) {
    int i;

    for (i = 0; i < ogrenciSayisi; i++) {
        if (ogrenciler[i].numara == numara) {
            return i;
        }
    }

    return -1;
}

void dosyayaKaydet() {
    FILE *dosya;
    int i;

    dosya = fopen(DOSYA_ADI, "w");

    if (dosya == NULL) {
        printf("Dosya acilamadi.\n");
        return;
    }

    for (i = 0; i < ogrenciSayisi; i++) {
        fprintf(dosya, "%d;%s;%s;%.2f;%.2f\n",
                ogrenciler[i].numara,
                ogrenciler[i].ad,
                ogrenciler[i].soyad,
                ogrenciler[i].vize,
                ogrenciler[i].final);
    }

    fclose(dosya);
}

void dosyadanOku() {
    FILE *dosya;

    dosya = fopen(DOSYA_ADI, "r");

    if (dosya == NULL) {
        return;
    }

    while (ogrenciSayisi < MAX_OGRENCI) {
        int okunan;

        okunan = fscanf(dosya, "%d;%49[^;];%49[^;];%f;%f\n",
                        &ogrenciler[ogrenciSayisi].numara,
                        ogrenciler[ogrenciSayisi].ad,
                        ogrenciler[ogrenciSayisi].soyad,
                        &ogrenciler[ogrenciSayisi].vize,
                        &ogrenciler[ogrenciSayisi].final);

        if (okunan != 5) {
            break;
        }

        ogrenciler[ogrenciSayisi].ortalama =
            ortalamaHesapla(ogrenciler[ogrenciSayisi].vize,
                            ogrenciler[ogrenciSayisi].final);

        harfNotuHesapla(ogrenciler[ogrenciSayisi].ortalama,
                        ogrenciler[ogrenciSayisi].harfNotu);

        ogrenciSayisi++;
    }

    fclose(dosya);
}

void bufferTemizle() {
    int karakter;

    while ((karakter = getchar()) != '\n' && karakter != EOF) {
    }
}