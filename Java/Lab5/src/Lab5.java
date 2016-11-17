import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import javax.swing.*;

abstract class Series {

    int base;
    int k;

    Series(int base, int k) {
        this.base = base;
        this.k = k;
    }

    abstract int getTerm(int j);

    public int getSum(int i, int j) throws Exception{
        if (i > j || i < 0 || j < 0) throw new Exception();
        int temp = 0;
        for (int t = i; t != j; t++)
            temp += getTerm(t);
        return temp;
    }

    public String toString() {
        StringBuffer str = new StringBuffer(toString(6));
        str.append("... ");
        return new String(str);
    }

    public String toString(int n) {
        StringBuffer str = new StringBuffer();
        for (int i = 0; i < n; i++) {
            str.append(Integer.toString(getTerm(i)));
            str.append(" ");
        }
        return new String(str);
    }

    public void write(String filename, int n) throws IOException {
        File out = new File(filename);
        if (!out.exists())
            out.createNewFile();
        PrintWriter printer = new PrintWriter(out.getAbsoluteFile());
        try {
            printer.print(toString(n));
        } finally {
            printer.close();
        }
    }

}

class Liner extends Series {

    Liner(int base, int k) {
        super(base, k);
    }

    int getTerm(int j) {
        return base + j * k;
    }
}

class Exponential extends Series {

    Exponential(int base, int k) {
        super(base, k);
    }

    int getTerm(int j) {
        int temp = 1;
        for (int i = 0; i < j; i++)
            temp *= k;
        return base * temp;
    }
}

class Window extends JFrame {

    private Series      series;
    private JButton      chooseLinerButton;
    private JButton      chooseExponentialButton;
    private JLabel       baseStaticLabel;
    private JLabel       coeffStaticLabel;
    private JLabel       seriesStaticLabel;
    private JTextField   seriesField;
    private JButton      seriesShowButton;
    private JLabel       seriesLabel;
    private JTextField   baseField;
    private JTextField   coefficientField;
    private JTextField   getTermField;
    private JButton      getTermButton;
    private JLabel       getTermLabel;
    private JButton      restartButton;
    private JLabel       exceptionLabel;
    private JTextField   getSumIField;
    private JTextField   getSumJField;
    private JButton      getSumButton;
    private JLabel       getSumLabel;
    private JButton      saveToFileButton;
    private JTextField   saveToFileField;
    private JTextField   saveToFileNumField;

    public Window() {
        super("Lab5");
        setBounds(400, 400, 600, 400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(null);
        setResizable(false);
        chooseLinerButton =         new JButton("Liner");
        chooseExponentialButton =   new JButton("Exponential");
        restartButton =             new JButton("Restart");
        seriesStaticLabel =         new JLabel("Series: ");
        seriesLabel =               new JLabel();
        seriesField =               new JTextField();
        seriesShowButton =          new JButton("Show");
        getTermField =              new JTextField();
        getTermButton =             new JButton("Get i term");
        getTermLabel =              new JLabel();
        baseStaticLabel =           new JLabel("Base: ");
        coeffStaticLabel =          new JLabel("Coeff: ");
        baseField =                 new JTextField();
        coefficientField =          new JTextField();
        exceptionLabel =            new JLabel("");
        getSumButton =              new JButton("Get sum");
        getSumIField =              new JTextField();
        getSumJField =              new JTextField();
        getSumLabel =               new JLabel();
        saveToFileButton =          new JButton("Save to");
        saveToFileField =           new JTextField();
        saveToFileNumField =        new JTextField();

        add(chooseLinerButton);
        add(chooseExponentialButton);
        add(restartButton);
        add(seriesStaticLabel);
        add(seriesLabel);
        add(seriesField);
        add(seriesShowButton);
        add(baseStaticLabel);
        add(coeffStaticLabel);
        add(baseField);
        add(coefficientField);
        add(exceptionLabel);
        add(getTermLabel);
        add(getTermField);
        add(getTermButton);
        add(getSumButton);
        add(getSumIField);
        add(getSumJField);
        add(getSumLabel);
        add(saveToFileButton);
        add(saveToFileField);
        add(saveToFileNumField);

        seriesStaticLabel.setBounds(170, 150, 70, 20);
        seriesShowButton.setBounds(220, 150, 75, 20);
        seriesField.setBounds(295, 150, 20, 20);
        seriesLabel.setBounds(320, 150, 270, 20);
        chooseLinerButton.setBounds(10, 10, 100, 40);
        chooseExponentialButton.setBounds(120, 10, 100, 40);
        restartButton.setBounds(480, 10, 100, 40);
        baseStaticLabel.setBounds(30, 150, 60, 20);
        coeffStaticLabel.setBounds(30, 190, 60, 20);
        baseField.setBounds(90, 150, 50, 20);
        coefficientField.setBounds(90, 190, 50, 20);
        exceptionLabel.setBounds(280, 10, 150, 40);
        getTermButton.setBounds(300, 190, 100, 20);
        getTermField.setBounds(400, 190, 40, 20);
        getTermLabel.setBounds(460, 190, 60, 20);
        getSumButton.setBounds(300, 220, 100, 20);
        getSumIField.setBounds(400, 220, 20, 20);
        getSumJField.setBounds(430, 220, 20, 20);
        getSumLabel.setBounds(460, 220, 80, 20);
        saveToFileButton.setBounds(60, 250, 80, 20);
        saveToFileField.setBounds(175, 250, 100, 20);
        saveToFileNumField.setBounds(150, 250, 20, 20);

        seriesShowButton.setEnabled(false);
        seriesField.setEnabled(false);
        getTermButton.setEnabled(false);
        getTermField.setEnabled(false);
        getSumIField.setEnabled(false);
        getSumJField.setEnabled(false);
        getSumButton.setEnabled(false);
        saveToFileButton.setEnabled(false);
        saveToFileNumField.setEnabled(false);
        saveToFileField.setEnabled(false);
        restartButton.setEnabled(false);

        ActionListener chooseTypeListener = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (series == null) {
                    try {
                        if (e.getSource() == chooseLinerButton)
                            series = new Liner(Integer.valueOf(baseField.getText()), Integer.valueOf(baseField.getText()));
                        else
                            series = new Exponential(Integer.valueOf(baseField.getText()), Integer.valueOf(baseField.getText()));
                        seriesLabel.setText(series.toString());
                        exceptionLabel.setText("");
                        seriesShowButton.setEnabled(true);
                        seriesField.setEnabled(true);
                        getTermButton.setEnabled(true);
                        getTermField.setEnabled(true);
                        getSumIField.setEnabled(true);
                        getSumJField.setEnabled(true);
                        getSumButton.setEnabled(true);
                        saveToFileButton.setEnabled(true);
                        saveToFileNumField.setEnabled(true);
                        saveToFileField.setEnabled(true);
                        restartButton.setEnabled(true);
                    } catch (NumberFormatException exc) {
                        exceptionLabel.setText("Base/Coeff incorrect");
                    }

                }
            }
        };

        chooseLinerButton.addActionListener(chooseTypeListener);
        chooseExponentialButton.addActionListener(chooseTypeListener);

        restartButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                seriesShowButton.setEnabled(false);
                seriesField.setEnabled(false);
                getTermButton.setEnabled(false);
                getTermField.setEnabled(false);
                getSumIField.setEnabled(false);
                getSumJField.setEnabled(false);
                getSumButton.setEnabled(false);
                saveToFileButton.setEnabled(false);
                saveToFileNumField.setEnabled(false);
                saveToFileField.setEnabled(false);
                restartButton.setEnabled(false);
                baseField.setText("");
                coefficientField.setText("");
                getTermLabel.setText("");
                getTermField.setText("");
                seriesLabel.setText("");
                seriesField.setText("");
                exceptionLabel.setText("");
                getTermField.setText("");
                getSumIField.setText("");
                getSumJField.setText("");
                getSumLabel.setText("");
                saveToFileNumField.setText("");
                saveToFileField.setText("");
                series = null;
            }
        });

        ActionListener methodListener = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (e.getSource() == seriesShowButton) {
                    if (series != null)
                        try {
                            if (seriesField.getText().compareTo("") == 0)
                                seriesLabel.setText(series.toString());
                            else
                                seriesLabel.setText(series.toString(Integer.valueOf(seriesField.getText())));
                            exceptionLabel.setText("");
                        } catch (NumberFormatException exc) {
                            exceptionLabel.setText("Show field incorrect");
                        }
                } else if (e.getSource() == getTermButton) {
                    if (series != null)
                        try {
                            getTermLabel.setText(Integer.toString(series.getTerm(Integer.valueOf(getTermField.getText()))));
                            exceptionLabel.setText("");
                        } catch(NumberFormatException exc) {
                            exceptionLabel.setText("Get term field incorrect");
                        }
                    return;
                } else if (e.getSource() == getSumButton) {
                    if (series != null)
                        try {
                            getSumLabel.setText(Integer.toString(series.getSum(Integer.valueOf(getSumIField.getText()), Integer.valueOf(getSumJField.getText()))));
                            exceptionLabel.setText("");
                        } catch(Exception exc) {
                            exceptionLabel.setText("Get sum field incorrect");
                        }
                } else if (e.getSource() == saveToFileButton) {
                    if (series != null)
                        try {
                            series.write(saveToFileField.getText(), Integer.valueOf(saveToFileNumField.getText()));
                            exceptionLabel.setText("");
                        } catch (IOException exc) {
                            exceptionLabel.setText("File name incorrect");
                        } catch (NumberFormatException exc) {
                            exceptionLabel.setText("Save to file field incorrect");
                        }
                }
            }
        };

        getTermButton.addActionListener(methodListener);
        getSumButton.addActionListener(methodListener);
        seriesShowButton.addActionListener(methodListener);
        saveToFileButton.addActionListener(methodListener);
    }

}

public class Lab5 {
    public static void main(String[] args) {
        Window wnd = new Window();
        wnd.setVisible(true);
    }
}
