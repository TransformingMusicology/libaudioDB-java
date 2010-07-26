import junit.framework.*;
import java.io.File;
import org.omras2.*;

public class TestQuery extends TestCase
{
	File testDBFile;
	File testFeatureFile;
	protected void setUp()
	{
		testDBFile = new File("testfiles/test.adb");
		testFeatureFile = new File("testfiles/testfeature");
		if(testDBFile.exists())
			testDBFile.delete();
	}

	public void testQuery()
	{
		// Insert the same feature twice
		AudioDB testDB = new AudioDB(testDBFile);
		assertTrue("DB created", testDB.create(1, 2, 1));
		testDB.open(AudioDB.Mode.O_RDWR);
		assertTrue("Insert feature file", testDB.insert("feat1", testFeatureFile));
		assertTrue("Insert feature file again", testDB.insert("feat2", testFeatureFile));
		testDB.close();

		testDB.open(AudioDB.Mode.O_RDONLY);
		Status status = testDB.getStatus();
		assertEquals("Two features", 2, status.getNumFiles());

		Query query = new Query();
		query.setSeqLength(1);
		query.setSeqStart(0);
		testDB.query("feat1", query);
	}

}
