# **Trees | `B` and `B+`**

This C++ implementation provides a `B` and `B+` tree implementation. Supports the following operations: `insert`, `search`, `range_search` and `pretty_print`.

## **Dataset: [Transactions](https://raw.githubusercontent.com/n4ndp/B-Trees/main/data/transactions.json)**

This dataset contains detailed information about transactions made by different users. The data is structured as follows:

```json
{
    "Alice": [
        {
            "TransactionID": "ALC20231224083000T001",
            "Timestamp": "2023-12-24T08:30:00Z",
            "Amount": 235.75,
            "Description": "Compra de víveres mensuales en supermercado XYZ, incluyendo productos frescos, abarrotes y artículos de limpieza"
        },
        {
            "TransactionID": "ALC20231224091500T002",
            "Timestamp": "2023-12-24T09:15:00Z",
            "Amount": 102.45,
            "Description": "Pago de factura de servicios públicos correspondiente al consumo de energía eléctrica y agua durante el mes anterior"
        },
        ...
    ],
    "Bob": [
        {
            "TransactionID": "BOB20231224114500T001",
            "Timestamp": "2023-12-24T11:45:00Z",
            "Amount": 78.60,
            "Description": "Compra de ropa de invierno en tienda de moda, incluyendo abrigos y accesorios"
        },
        ...
    ],
    ...
}
```

## **B Tree**

```textplain
      78.6: (Bob, BOB20231224114500T001)
   78.9: (Alice, ALC20231224141500T005)
      89.99: (Charlie, CHR20231224120000T002)
   95.25: (Bob, BOB20231224203000T005)
      102.45: (Alice, ALC20231224091500T002)
      112.7: (Charlie, CHR20231224141500T003)
150: (Alice, ALC20231224120000T004)
      150.3: (Bob, BOB20231224132000T002)
      175.8: (Charlie, CHR20231224164500T004)
   185.2: (Alice, ALC20231224103000T003)
      210.25: (Charlie, CHR20231224203000T005)
      235.75: (Alice, ALC20231224083000T001)
   245.8: (Bob, BOB20231224150000T003)
      320.15: (Charlie, CHR20231224093000T001)
      320.45: (Bob, BOB20231224171500T004)
```

## **B+ Tree**

```textplain
      78.6: (Bob, BOB20231224114500T001)
      78.9: (Alice, ALC20231224141500T005)
  89.99
      89.99: (Charlie, CHR20231224120000T002)
      95.25: (Bob, BOB20231224203000T005)
102.45
      102.45: (Alice, ALC20231224091500T002)
      112.7: (Charlie, CHR20231224141500T003)
  150
      150: (Alice, ALC20231224120000T004)
      150.3: (Bob, BOB20231224132000T002)
      175.8: (Charlie, CHR20231224164500T004)
  185.2
      185.2: (Alice, ALC20231224103000T003)
      210.25: (Charlie, CHR20231224203000T005)
      235.75: (Alice, ALC20231224083000T001)
  245.8
      245.8: (Bob, BOB20231224150000T003)
      320.15: (Charlie, CHR20231224093000T001)
      320.45: (Bob, BOB20231224171500T004)
```
