#!/bin/bash

# Verificar si se proporcionó un argumento
if [ -z "$1" ]; then
    echo "Uso: $0 <número de archivos>"
    exit 1
fi

# Número de archivos a crear
N=$1

# Función para generar contenido aleatorio
generar_contenido_random() {
    cat /dev/urandom | tr -dc 'a-zA-Z0-9' | head -c 100
}

# Crear archivos con contenido específico y aleatorio
for (( i=1; i<=N; i++ ))
do
    filename="Block${i}.txt"
    encabezado="data from block ${i}:"
    contenido=$(generar_contenido_random)
    echo "$encabezado" > "$filename"
    echo "$contenido" >> "$filename"
    echo "Creado: $filename con encabezado y contenido aleatorio."
done

echo "Proceso completado. Se han creado $N archivos con encabezado y contenido aleatorio."
